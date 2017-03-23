#include "rti_impl.h"
#include "rti_interfaces.h"
#include "boost/thread.hpp"
#include <boost/thread/mutex.hpp>
#include <string>
#include <sys/time.h>

using namespace std;
using namespace rti;

boost::mutex io_mutex;

#ifndef BUFF_SIZE
#define BUFF_SIZE 1024*1024
#endif


// TODO: message should be split (larger than 1024 * 1024)

class PingSender : public IDataSink {
private:
    boost::thread* t;
    IDataSink* ping_sink;
    int ping_count;

    DdsConnection* conn;
    timeval call_time;
    char* buff;

public:
    PingSender() : ping_count(0),
                   buff(new char[BUFF_SIZE]) {
        ping_sink = new DdsDataSender(100, "ping-forth");
        conn = new DdsConnection(this, 200, "ping-back", 128);
        for (int i = 0; i < BUFF_SIZE; i++)
            buff[i] = (char)(i % 255);
    }
    void start_loop() {
        t = new boost::thread(&PingSender::loop, this);
    }

    void loop() {
        while (1) {
            string str(SSTR("ping " << ping_count++));
            {
                boost::unique_lock<boost::mutex> scoped_lock(
                    io_mutex);
                cout << "sending: " << str << endl;
            }

            gettimeofday(&call_time, NULL);
            sprintf(buff, "%s", str.c_str());
            ping_sink->sink(buff, BUFF_SIZE, AUTO_MSG_ID, true);
            sleep(1);
        }
    }

    void join() {
        t->join();
    }

    virtual void sink(char* data, int len, int msgId, bool isLast) {
        timeval resp_time;
        gettimeofday(&resp_time, NULL);
        double tt = GET_TDIFF(call_time, resp_time);
        string str(data);
        
        {
            boost::unique_lock<boost::mutex> scoped_lock(io_mutex);
            cout << "received (" << tt << "): "<< str << endl;
        }
    }
};

class PingReceiver : public IDataSink {
private:
    boost::thread* t;
    IDataSink* back_sink;
    DdsConnection* conn;
    char* buff;

public:
    PingReceiver() : buff(new char[BUFF_SIZE]) {
        back_sink = new DdsDataSender(200, "ping-back");
        conn = new DdsConnection(this, 100, "ping-forth", 128);
        for (int i = 0; i < BUFF_SIZE; i++)
            buff[i] = (char)(i % 255);
    }
    
    virtual void sink(char* data, int len, int msgId, bool isLast) {
        string str(data);
        {
            boost::unique_lock<boost::mutex> scoped_lock(io_mutex);
            cout << "answering: " << str << endl;
        }

        sprintf(buff, "reply to %s", str.c_str());
        back_sink->sink(buff, BUFF_SIZE, AUTO_MSG_ID, true);
    }

};

int main() {
    
    PingReceiver pr;
    PingSender ps;
    ps.start_loop();
    ps.join();
    return 0;
}
