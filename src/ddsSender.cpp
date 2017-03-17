#include "ros/ros.h"
#include "ros/serialization.h"
#include <image_transport/image_transport.h>
#include <ardrone_autonomy/Navdata.h>
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"

// TODO: random node id
// TODO: add log infos
// TODO:     nodeHandle.getParam("/ddsNavDataDomain", navDataDomain);
// TODO: errors may arise
// TODO: add last image sent stats
// TODO: catch exceptions

class RosSender {
private:
    int rosRate;
    ros::NodeHandle nodeHandle;
    rti::IDataSink* imageSender;
    rti::IDataSink* navDataSender;
    ros::Subscriber vid_sub;
    ros::Subscriber navdata_sub;

public:
    RosSender(rti::IDataSink imageSender,
              rti::IDataSink navDataSender) :
        rosRate(30),
        nodeHandle(),
        imageSender(imageSender),
        navDataSender(navDataSender);
    virtual ~RosSender();
    int loop();

public:
    void imageMessageCallback(const sensor_msgs::ImageConstPtr msg);
    void navdataMessageCallback(const ardrone_autonomy::NavdataConstPtr msg);
};

RosSender::RosSender(rti::IDataSender sender) {
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

void RosSender::imageMessageCallback(const sensor_msgs::ImageConstPtr msg) {
    uint32_t serial_size = ros::serialization::serializationLength(*msg);
    boost::shared_array<uint8_t> obuffer(new uint8_t[serial_size]);

    ros::serialization::OStream ostream(obuffer.get(), serial_size);
    ros::serialization::serialize(ostream, *msg);

    imageSender->sink(obuffer.get(), serial_size, AUTO_MSG_ID);
}

void RosSender::navdataMessageCallback(const ardrone_autonomy::NavdataConstPtr msg) {
    uint32_t serial_size = ros::serialization::serializationLength(*mpsg);
    boost::shared_array<uint8_t> obuffer(new uint8_t[serial_size]);

    ros::serialization::OStream ostream(obuffer.get(), serial_size);
    ros::serialization::serialize(ostream, *msg);

    navDataSender->sink(obuffer.get(), serial_size);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsSender");
    rti::DdsDataSender ddsImageSender;
    rti::DdsDataSender ddsNavDataSender;
    RosSender rs(&ddsImageSender, &ddsNavDataSender);
    return rs.loop();
}

