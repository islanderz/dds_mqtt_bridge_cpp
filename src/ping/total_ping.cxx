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

class WifiPingerReceiver : public IDataSink {
private:
    PingStats* ps;    
    DdsConnection* conn;
public:
    WifiPingerReceiver(PingStats* ps) : ps(ps) {
        conn = new DdsConnection(this, 200, "wifi-ping-stats", 128);
    }

    virtual void sink(char* data, int len, int msgId, bool isLast) {
        double* data2 = (double*)data;
        ps->write_stat(WIFI_500, data2[0]);
        ps->write_stat(WIFI_2K, data2[1]);
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
    
    WifiPingerReceiver wpr(&ps);
    DdsPinger dds(&ps);

    // need to allow dds subscriptions to settle
    sleep(1);
    
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
