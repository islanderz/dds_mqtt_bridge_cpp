#include "ros/ros.h"
#include "ros/serialization.h"
#include <image_transport/image_transport.h>
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"

using namespace rti;

// TODO: random node id
// TODO: add log infos
// TODO:     nodeHandle.getParam("/ddsNavDataDomain", navDataDomain);
// TODO: errors may arise
// TODO: add last image sent stats
// TODO: optimize buffer operations (!!!)
// TODO: consider threading model once more, mutexes, etc.
// TODO: message number
// TODO: fix sleep

class RosReceiver : public IDataSink {
private:
    int rosRate;
    ros::NodeHandle nodeHandle;
    image_transport::Publisher imagePub;

public:
    RosReceiver();
    virtual ~RosReceiver();

public:
    virtual void sink(char* buffer, int len, int msgId, bool isLast);
};

RosReceiver::RosReceiver() :
    rosRate(30),
    nodeHandle() {
    image_transport::ImageTransport img_trans(nodeHandle);    
    imagePub = img_trans.advertise("/ardrone/image_raw", 1);
}

RosReceiver::~RosReceiver() {
}

void RosReceiver::sink(char* buffer, int len, int msgId,
    bool isLast) {
    // assumption is that message is already assembled
    sensor_msgs::Image image_msg;
    ros::serialization::IStream istream((uint8_t*)buffer, len);
    ros::serialization::deserialize(istream, image_msg);

    imagePub.publish(image_msg);
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsImageReceiver");

    RosReceiver rr;
    DdsConnection conn(&rr, 100, "ros-images", 1024*1024);
    return conn.loop(100000);
}

