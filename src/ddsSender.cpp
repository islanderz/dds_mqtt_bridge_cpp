#include "ros/ros.h"
#include "ros/serialization.h"
#include <image_transport/image_transport.h>
#include <ardrone_autonomy/Navdata.h>
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"

using namespace rti;

// TODO: random node id
// TODO: add log infos
// TODO:     nodeHandle.getParam("/ddsNavDataDomain", navDataDomain);
// TODO: errors may arise
// TODO: add last image sent stats
// TODO: catch exceptions
// TODO: remove MQTT files from the project

class RosSender {
private:
    int rosRate;
    ros::NodeHandle nodeHandle;
    IDataSink* imageSender;
    IDataSink* navDataSender;
    ros::Subscriber vid_sub;
    ros::Subscriber navdata_sub;
    char* buff;

public:
    RosSender(IDataSink* imageSender,
        IDataSink* navDataSender);
    virtual ~RosSender();
    int loop();

public:
    void imageMessageCallback(const sensor_msgs::ImageConstPtr msg);
    void navdataMessageCallback(const ardrone_autonomy::NavdataConstPtr msg);
};

RosSender::RosSender(IDataSink* imageSender,
    IDataSink* navDataSender) :
    rosRate(30),
    nodeHandle(),
    imageSender(imageSender),
    navDataSender(navDataSender),
    buff(new char[1024*1024]) {
    
    vid_sub = nodeHandle.subscribe
        ("/ardrone/image_raw",
         10,
         &RosSender::imageMessageCallback,
         this);
    navdata_sub = nodeHandle.subscribe
        ("/ardrone/navdata",
         10,
         &RosSender::navdataMessageCallback,
         this);
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

void RosSender::imageMessageCallback(
    const sensor_msgs::ImageConstPtr msg) {
    uint32_t serial_size = ros::serialization::serializationLength(
        *msg);

    if (serial_size > 1024*1024) {
        printf("too large image");
        exit(1);
    }
    ros::serialization::OStream ostream((uint8_t*)buff, serial_size);
    ros::serialization::serialize(ostream, *msg);
    imageSender->sink(buff, (int)serial_size,
        AUTO_MSG_ID, true);
}

void RosSender::navdataMessageCallback(
    const ardrone_autonomy::NavdataConstPtr msg) {
    uint32_t serial_size = ros::serialization::serializationLength(
        *msg);

    if (serial_size > 1024*1024) {
        printf("too large navdata");
        exit(1);
    }
        
    ros::serialization::OStream ostream((uint8_t*)buff, serial_size);
    ros::serialization::serialize(ostream, *msg);
    navDataSender->sink(buff, (int)serial_size,
        AUTO_MSG_ID, true);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsSender");
    DdsDataSender ddsImageSender(100, "ros-images");
    DdsDataSender ddsNavDataSender(200, "ros-navdata");
    RosSender rs(&ddsImageSender, &ddsNavDataSender);
    return rs.loop();
}

