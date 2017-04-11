#include "ros/ros.h"
#include "rti_impl.h"
#include "rti_interfaces.h"
#include "boost/thread.hpp"
#include <boost/thread/mutex.hpp>
#include <string>
#include <sys/time.h>
#include <signal.h>

using namespace std;
using namespace rti;

#define WIFI_IP "192.168.1.1"
//#define WIFI_IP "localhost"

bool shutdown_total_ping = false;
void sigint_handler(int signo) {
    if (signo == SIGINT)
        shutdown_total_ping = true;
}

class PingReceiver : public IDataSink {
private:
    boost::thread* t;
    IDataSink* back_sink;
    DdsConnection* conn;
    int buff_size;
    char* buff;

public:
    PingReceiver(int buff_size, string incoming_queue,
        string outgoing_queue) : buff_size(buff_size),
                                 buff(new char[buff_size]) {
        back_sink = new DdsDataSender(100, outgoing_queue);
        conn = new DdsConnection(this, 100, incoming_queue, 128);
        for (int i = 0; i < buff_size; i++)
            buff[i] = (char)(i % 255);
    }
    
    virtual void sink(char* data, int len, int msgId, bool isLast) {
        string str(data);
        string ping;
        int ping_num;
        stringstream ss(str);
        ss >> ping >> ping_num;
        sprintf(buff, "pong %d", ping_num);
        back_sink->sink(buff, buff_size, AUTO_MSG_ID, true);
    }
};

class WifiPinger {
private:
    boost::thread* t;
    IDataSink* stat_sink;

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
    WifiPinger() {
        stat_sink = new DdsDataSender(200, "wifi-ping-stats");
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

        int sbuf_size = sizeof(double) * 2;
        double sendbuff[2];

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

            // update Dont smooth out here
            // p500 = 0.7 * p500 + 0.3 * res500;
            // p20000 = 0.7 * p20000 + 0.3 * res20000;

            // send
            sendbuff[0] = (double)p500;
            sendbuff[1] = (double)p20000;
            stat_sink->sink((char*)sendbuff, sbuf_size,
                AUTO_MSG_ID, true);
            
            // sleep 1s
            r.sleep();
            if(!ros::ok()) break;
            r.sleep();
            if(!ros::ok()) break;
        }
    }

};

int main(int argc, char **argv) {
    ros::init(argc, argv, "total_ping");
    ros::NodeHandle nodeHandle;

    PingReceiver pr500(500, "ping-forth-500", "ping-back-500");
    PingReceiver pr2k(20000, "ping-forth-2k", "ping-back-2k");

    WifiPinger wp;
    wp.start();

    signal(SIGINT, sigint_handler);

    while (!shutdown_total_ping)
        sleep(1000000);
    return 0;
}
