#include "ros/ros.h"
#include "ros/serialization.h"
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"

#include "rti_dds.h"
#include "rti_ddsSupport.h"
#include "ndds/ndds_cpp.h"

using namespace rti;

class StreamReceiver : public IDataSink {
private:
    DdsConnection* conn;
protected:
    ros::Publisher publisher;
public:
    StreamReceiver(ros::Publisher publisher, string topic,
        int domainId, int buffSize);
    virtual ~StreamReceiver();
    virtual void sink(char* buffer, int len, int msgId, bool isLast) = 0;
};

StreamReceiver::StreamReceiver(ros::Publisher publisher,
    string topic, int domainId, int buffSize) :
    publisher(publisher),
    conn(new DdsConnection(this, domainId, topic, buffSize)) {
}

StreamReceiver::~StreamReceiver() {
    /* omitting for performance reasons
    delete conn;
    */
}

class SimpleStreamReceiver : public StreamReceiver {
public:
    SimpleStreamReceiver(ros::Publisher publisher, string topic,
        int domainId);
    virtual void sink(char* buffer, int len, int msgId, bool isLast);
};

SimpleStreamReceiver::SimpleStreamReceiver(ros::Publisher publisher,
    string topic, int domainId) :
    StreamReceiver(publisher, topic, domainId, 8) {
}

void SimpleStreamReceiver::sink(char* buffer, int len, int msgId, bool isLast) {
    publisher.publish(std_msgs::Empty());
}

class CmdVelStreamReceiver : public StreamReceiver {
public:
    CmdVelStreamReceiver(ros::Publisher publisher, string topic,
        int domainId);
    virtual void sink(char* buffer, int len, int msgId, bool isLast);
};

CmdVelStreamReceiver::CmdVelStreamReceiver(ros::Publisher publisher,
    string topic, int domainId) :
    StreamReceiver(publisher, topic, domainId, 1024*1024) {
}
void CmdVelStreamReceiver::sink(char* buffer, int len, int msgId,
    bool isLast) {
    geometry_msgs::Twist cmd_vel_msg;
    ros::serialization::IStream istream((uint8_t*)buffer, len);
    ros::serialization::deserialize(istream, cmd_vel_msg);
    publisher.publish(cmd_vel_msg);
}

class RosReceiver {
private:
    ros::NodeHandle nodeHandle;
#define RECEIVER_COUNT 4
    StreamReceiver* receivers[4];


public:
    RosReceiver();
    virtual ~RosReceiver();

    void loop();
};

RosReceiver::RosReceiver() :
    nodeHandle() {

    int zz = 0;
    receivers[zz++] = new SimpleStreamReceiver(
        nodeHandle.advertise<std_msgs::Empty>("/ardrone/land", 1),
        "ros-land", 201);
    receivers[zz++] = new SimpleStreamReceiver(
        nodeHandle.advertise<std_msgs::Empty>("/ardrone/reset", 1),
        "ros-reset", 202);
    receivers[zz++] = new SimpleStreamReceiver(
        nodeHandle.advertise<std_msgs::Empty>("/ardrone/takeoff", 1),
        "ros-takeoff", 203);
    receivers[zz++] = new CmdVelStreamReceiver(
        nodeHandle.advertise<geometry_msgs::Twist>("/cmd_vel", 1),
        "ros-cmdvel", 204);
}

RosReceiver::~RosReceiver() {
    // TODO: stop receivers
    for (int i = 0; i < RECEIVER_COUNT; i++)
        delete receivers[i];
}

void RosReceiver::loop() {
    pause();
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsCmdVelReceiver");

    RosReceiver rr;
    rr.loop();
}


