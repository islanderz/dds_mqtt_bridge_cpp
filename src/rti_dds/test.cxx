#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"
#include "boost/thread.hpp"
#include <boost/thread/mutex.hpp>
#include <string>

using namespace std;
using namespace rti;

boost::mutex io_mutex;

class PingSender : public IDataSink {
private:
    boost::thread* t;
    IDataLink* ping_sink;
    int ping_count;

    DdsConnection* conn;
public:
    PingSender() : ping_count(0) {
        ping_sink = new DataSender(100, "ping-forth");
        conn = new DdsConnection conn(this, 200, "ping-back", 128);
    }
    void start_loop() {
        t = new boost::thread(&PingSender::loop, this);
    }

    void loop() {
        while (1) {
            string str(SSTR("ping " << ping_count++));
            {
                boost::unique_lock<boost::mutex> scoped_lock(io_mutex);
                cout << "sending: " << str << endl;
            }
            ping_sink->sink(str.c_str(), str.length, AUTO_MSG_ID);
            sleep(1);
        }
    }

    virtual void sink(char* data, int len, int msgId) {
        string str(data, len);
        {
            boost::unique_lock<boost::mutex> scoped_lock(io_mutex);
            cout << "received: " << str << endl;
        }
    }
};

class PingReceiver : public IDataSink {
private:
    boost::thread* t;
    IDataLink* back_sink;
    DdsConnection* conn;
public:
    PingReceiver() {
        back_sink = new DataSender(200, "ping-back");
        conn = new DdsConnection conn(this, 100, "ping-forth", 128);
    }
    
    virtual void sink(char* data, int len, int msgId) {
        string str(data, len);
        {
            boost::unique_lock<boost::mutex> scoped_lock(io_mutex);
            cout << "answering: " << str << endl;
        }
        string answer(SSTR("reply to " << str));
        back_sink->sink(answer.c_str(), answer.length, AUTO_MSG_ID);
    }

};

int main() {
    PingReceiver pr;
    PingSender ps;
    ps.start_loop();
    return 0;
}
