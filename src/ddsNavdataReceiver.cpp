#include "ros/ros.h"
#include "ros/serialization.h"
#include <ardrone_autonomy/Navdata.h>
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"

using namespace rti;

// TODO: random node id
// TODO: add log infos
// TODO:     nodeHandle.getParam("/ddsNavDataDomain", navDataDomain);
// TODO: errors may arise
// TODO: add last image sent stats
// TODO: optimize buffer operations
// TODO: consider threading model once more, mutexes, etc.
// TODO: message number

class RosReceiver : public IDataSink {
private:
    int rosRate;
    ros::NodeHandle nodeHandle;
    ros::Publisher navdataPub;

public:
    RosReceiver() :
        rosRate(30),
        nodeHandle();
    virtual ~RosReceiver();

public:
    virtual int sink(char* buffer, int len, bool isLast);
};

RosReceiver::RosReceiver() {
    navdataPub = nodeHandle.advertise<ardrone_autonomy::Navdata>
        ("/tum_ardrone/navdata", 1);
}

RosReceiver::~RosReceiver() {
}

int RosReceiver::sink(char* buffer, int len, int msgId,
    bool isLast) {
    // assumption is that message is already assembled
    ardrone_autonomy::Navdata navMsg;
    boost::shared_array<uint8_t> ibuffer(new uint8_t[len]);
    ros::serialization::IStream istream(buffer, len);
    ros::serialization::deserialize(istream, navMsg);
    navdataPub.publish(navMsg);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsNavdataReceiver");

    RosReceiver rr();
    DdsConnection conn(&rr);
    return conn.loop();
}

