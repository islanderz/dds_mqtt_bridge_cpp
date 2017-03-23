#include "ros/ros.h"
#include "rti_impl.h"
#include "rti_interfaces.h"
#include "boost/thread.hpp"
#include <boost/thread/mutex.hpp>
#include <string>
#include <sys/time.h>
#include <iostream>

using namespace std;
using namespace rti;

#define WIFI_500    0
#define WIFI_2K     1
#define DDS_500     2
#define DDS_2K      3
#define STAT_LEN    4

#define WIFI_IP 192.168.1.1


class PingStats {
private:
    boost::mutex io_mutex;
    double values[STAT_LEN];
public:
    
    void write_stat(int stat_pos, double value) {
        boost::unique_lock<boost::mutex> scoped_lock(
            io_mutex);
        values[stat_pos] = value;
    }
    
    void get_values(double *output_values) {
        boost::unique_lock<boost::mutex> scoped_lock(
            io_mutex);
        memcpy((char*)output_values, (char*)values,
            STAT_LEN * sizeof(double));
    }
};

class WifiPinger {
private:
    PingStats* ps;
    boost::thread* t;

    float parsePingResult(std::string s)
    {
        // 20008 bytes from localhost (127.0.0.1): icmp_req=1 ttl=64 time=0.075 ms
        int pos = s.find("time=");
        int found = 0;
        float ms;
        if(pos != std::string::npos)
            found = sscanf(s.substr(pos).c_str(),"time=%f",&ms);

        if(found == 1 && pos != std::string::npos)
            return ms;
        else
            return 10000;
    }


public:
    WifiPinger(PingStats* ps) : ps(ps) {
    }

    void start() {
        t = new boost::thread(&WifiPinger::ping_function, this);
    }

    void ping_function()
    {
        float p500;
        float p20000;
        char pingCommand500[100];
        char pingCommand20000[100];
        char line1[200];
        char line2[200];

        sprintf(pingCommand20000,"ping -c 1 -s 20000 -w 1 " WIFI_IP);
        sprintf(pingCommand500,"ping -c 1 -s 500 -w 1 " WIFI_IP);
        ros::Rate r(2.0);
        FILE *p;

        while(ros::ok())
        {
            // ping twice, with a sleep in between
            p = popen(pingCommand500,"r");
            fgets(line1, 200, p);
            fgets(line1, 200, p);
            pclose(p);

            // sleep 1s
            r.sleep();
            if(!ros::ok()) break;
            r.sleep();
            if(!ros::ok()) break;

            p = popen(pingCommand20000,"r");
            fgets(line2, 200, p);
            fgets(line2, 200, p);
            pclose(p);

            // parse results which should be in line1 and line2
            float res500 = parsePingResult(line1);
            float res20000 = parsePingResult(line2);


            // clip between 10 and 1000.
            res500 = min(1000.0f,max(10.0f,res500));
            res20000 = min(1000.0f,max(10.0f,res20000));

            // update
            p500 = 0.7 * p500 + 0.3 * res500;
            p20000 = 0.7 * p20000 + 0.3 * res20000;

            // send
            ps->write_stat(WIFI_500, (double)p500);
            ps->write_stat(WIFI_500, (double)p20000);

            // sleep 1s
            r.sleep();
            if(!ros::ok()) break;
            r.sleep();
            if(!ros::ok()) break;
        }
    }

};

class DdsPinger : public IDataSink {
private:
    PingStats* ps;
    boost::thread* t;
    int ping_count;
    int ping_rec;
    timeval call_time;
    timeval resp_time;
    int expected_len;

    IDataSink* ping_sink500;
    DdsConnection* conn500;
    char* buff500;

    IDataSink* ping_sink2k;
    DdsConnection* conn2k;
    char* buff2k;

    boost::mutex ps_mutex;
    boost::condition_variable ps_cond;

    void ping_step(int stat_pos, char *buff, int buff_size,
        IDataSink* ping_sink) {
        boost::unique_lock<boost::mutex> lock(ps_mutex);

        expected_len = buff_size;
        ping_count++;
        gettimeofday(&call_time, NULL);
        sprintf(buff, "ping %d", ping_count);
        ping_sink->sink(buff, buff_size, AUTO_MSG_ID, true);

        while (ping_count != ping_rec)
            ps_cond.wait(lock);

        double tt = GET_TDIFF(call_time, resp_time);
        ps->write_stat(stat_pos, tt);
        usleep(200000);
    }

public:
    DdsPinger(PingStats* ps) :
        ps(ps),
        ping_count(0),
        ping_rec(0),
        buff500(new char[500]),
        buff2k(new char[20000]) {

        ping_sink500 = new DdsDataSender(100, "ping-forth-500");
        conn500 = new DdsConnection(this, 100, "ping-back-500", 128);

        ping_sink2k = new DdsDataSender(100, "ping-forth-2k");
        conn2k = new DdsConnection(this, 100, "ping-back-2k", 128);

        for (int i = 0; i < 500; i++)
            buff500[i] = (char)(i % 255);
        for (int i = 0; i < 20000; i++)
            buff2k[i] = (char)(i % 255);
    }

    void start() {
        t = new boost::thread(&DdsPinger::ping_function, this);
    }

    void ping_function() {
        while (1) {
            ping_step(DDS_500, buff500, 500, ping_sink500);
            ping_step(DDS_2K, buff2k, 20000, ping_sink2k);
        }
    }

    virtual void sink(char* data, int len, int msgId, bool isLast) {
        boost::unique_lock<boost::mutex> lock(ps_mutex);
        gettimeofday(&resp_time, NULL);
        
        if (len != expected_len)
            cerr << "bad buffer length" << endl;
        
        string str(data);
        if (str.compare(SSTR("pong " << ping_count)) != 0) 
            cerr << "bad ping number" << endl;

        ping_rec++;
        ps_cond.notify_one();
    }

};


int main(int argc, char **argv) {
    ros::init(argc, argv, "total_ping");
    ros::NodeHandle nodeHandle;

    PingStats ps;

    WifiPinger wp(&ps);
    DdsPinger dds(&ps);

    // need to allow dds subscriptions to settle
    sleep(1);

    wp.start();
    dds.start();
    
    double stats[STAT_LEN];
    while (1) {
        sleep(1);
        ps.get_values(stats);
        for (int i = 0; i < STAT_LEN; i++)
            cerr << fixed << stats[i] << " ";
        cerr << endl;
    }
    return 0;
}
