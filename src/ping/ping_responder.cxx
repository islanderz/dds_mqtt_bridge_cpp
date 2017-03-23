#include "rti_impl.h"
#include "rti_interfaces.h"
#include "boost/thread.hpp"
#include <boost/thread/mutex.hpp>
#include <string>
#include <sys/time.h>

using namespace std;
using namespace rti;

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

int main(int argc, char **argv) {
    PingReceiver pr500(500, "ping-forth-500", "ping-back-500");
    PingReceiver pr2k(20000, "ping-forth-2k", "ping-back-2k");

    while (1)
        sleep(1000000);
    return 0;
}
