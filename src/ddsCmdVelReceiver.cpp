#include "ros/ros.h"
#include "ros/serialization.h"
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"

using namespace rti;

// TODO: random node id
// TODO: add log infos
// TODO:     nodeHandle.getParam("/ddsNavDataDomain", navDataDomain);
// TODO: errors may arise
// TODO: add last image sent stats
// TODO: optimize buffer operations (!!!)
// TODO: consider threading model once more, mutexes, etc.
// TODO: message number

class RosReceiver : public IDataSink {
private:
    int rosRate;
    ros::NodeHandle nodeHandle;
    ros::Publisher cmdVelPub;
    ros::Publisher resetPub;
    ros::Publisher landPub;
    ros::Publisher takeoffPub;

public:
    RosReceiver();
    virtual ~RosReceiver();

public:
    virtual void sink(char* buffer, int len, int msgId, bool isLast);
};

RosReceiver::RosReceiver() :
    rosRate(30),
    nodeHandle() {

	cmdVelPub = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1); 
    landPub = nh_.advertise<std_msgs::Empty>("/ardrone/land", 1);
    resetPub = nh_.advertise<std_msgs::Empty>("/ardrone/reset", 1);
    takeoffPub = nh_.advertise<std_msgs::Empty>(
        "/ardrone/takeoff", 1);
}

RosReceiver::~RosReceiver() {
}

void RosReceiver::sink(char* buffer, int len, int msgId,
    bool isLast) {
    
    // assumption is that message is already assembled

    ros::serialization::IStream istream((uint8_t*)buffer, len);
    ros::serialization::deserialize(istream, navMsg);

    //    navdataPub.publish(navMsg);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsImageReceiver");

    RosReceiver rr;
    DdsConnection conn(&rr, 200, "ros-navdata", 1024*1024);
    return conn.loop(-1);
}

