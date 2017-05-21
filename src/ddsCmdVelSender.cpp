#include "ros/ros.h"
#include "ros/serialization.h"
#include <image_transport/image_transport.h>
#include <ardrone_autonomy/Navdata.h>
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"

using namespace rti;

class RosSender {
private:
    int rosRate;
    ros::NodeHandle nodeHandle;
    IDataSink* landSender;
    IDataSink* resetSender;
    IDataSink* takeoffSender;
    IDataSink* cmdvelSender;

    ros::Subscriber land_sub;
    ros::Subscriber reset_sub;
    ros::Subscriber takeoff_sub;
    ros::Subscriber cmdvel_sub;

    char* buff;
    char dummy[2];

public:
    RosSender(IDataSink* landSender, IDataSink* resetSender,
        IDataSink* takeoffSender, IDataSink* cmdvelSender);
    virtual ~RosSender();
    int loop();

public:
    void landMessageCallback(const std_msgs::EmptyConstPtr);
    void resetMessageCallback(const std_msgs::EmptyConstPtr);
    void takeoffMessageCallback(const std_msgs::EmptyConstPtr);
    void cmdvelMessageCallback(const geometry_msgs::Twist msg);
};

RosSender::RosSender(IDataSink* landSender, IDataSink* resetSender,
        IDataSink* takeoffSender, IDataSink* cmdvelSender) :
    rosRate(30),
    nodeHandle(),
    landSender(landSender),
    resetSender(resetSender),
    takeoffSender(takeoffSender),
    cmdvelSender(cmdvelSender),
    buff(new char[1024*1024]) {

    dummy[0] = '#';
    dummy[1] = '#';

    land_sub = nodeHandle.subscribe
        ("/ardrone/land",
         1000,
         &RosSender::landMessageCallback,
         this);
    reset_sub = nodeHandle.subscribe
        ("/ardrone/reset",
         1000,
         &RosSender::resetMessageCallback,
         this);
    takeoff_sub = nodeHandle.subscribe
        ("/ardrone/takeoff",
         1000,
         &RosSender::takeoffMessageCallback,
         this);
    cmdvel_sub = nodeHandle.subscribe
        ("/cmd_vel",
         1000,
         &RosSender::cmdvelMessageCallback,
         this);
}

void RosSender::landMessageCallback(const std_msgs::EmptyConstPtr) {
    landSender->sink(dummy, 2, AUTO_MSG_ID, true);
}
void RosSender::resetMessageCallback(const std_msgs::EmptyConstPtr) {
    resetSender->sink(dummy, 2, AUTO_MSG_ID, true);
}
void RosSender::takeoffMessageCallback(
    const std_msgs::EmptyConstPtr) {
    takeoffSender->sink(dummy, 2, AUTO_MSG_ID, true);
}
void RosSender::cmdvelMessageCallback(
    const geometry_msgs::Twist msg) {
    uint32_t serial_size = ros::serialization::serializationLength(
        msg);

    if (serial_size > 1024*1024) {
        printf("too large cmdvel data");
        exit(1);
    }

    boost::shared_array<uint8_t> obuffer(new uint8_t[serial_size]);

    ros::serialization::OStream ostream((uint8_t*)buff, serial_size);
    ros::serialization::serialize(ostream, msg);

    cmdvelSender->sink(buff, (int)serial_size, AUTO_MSG_ID, true);
}


int RosSender::loop() {
    ros::Rate rate(rosRate);
    while (ros::ok()) {
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

RosSender::~RosSender() {
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsSender");
    DdsDataSender ddsLandSender(201, "ros-land");
    DdsDataSender ddsResetSender(202, "ros-reset");
    DdsDataSender ddsTakeoffSender(203, "ros-takeoff");
    DdsDataSender ddsCmdVelSender(204, "ros-cmdvel");

    RosSender rs(&ddsLandSender, &ddsResetSender,
        &ddsTakeoffSender, &ddsCmdVelSender);
    return rs.loop();
}

