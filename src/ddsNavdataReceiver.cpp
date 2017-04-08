#include "ros/ros.h"
#include "ros/serialization.h"
#include <ardrone_autonomy/Navdata.h>
#include "rti_dds/rti_impl.h"
#include "rti_dds/rti_interfaces.h"
#include <fstream>

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
    ros::Publisher navdataPub;
    timeval last_img_time;
    int frame_num;
    std::ofstream log_file_ros_navdata_recv;	
    double time_spent;
public:
    RosReceiver();
    virtual ~RosReceiver();

public:
    virtual void sink(char* buffer, int len, int msgId, bool isLast);
};

RosReceiver::RosReceiver() :
    rosRate(200),
    nodeHandle(),
    frame_num(0),
    time_spent(0) {

    gettimeofday(&last_img_time, NULL);
    
    navdataPub = nodeHandle.advertise<ardrone_autonomy::Navdata>
        ("/tum_ardrone/navdata", 1);

    std::stringstream ss;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d_%m_%Y_%H_%M_%S",timeinfo);
    std::string date_time(buffer);

    ss << "log_file_dds_navdata_av_delays_" << date_time << ".txt";
    log_file_ros_navdata_recv.open(ss.str(), std::ofstream::out | std::ofstream::app);
}

RosReceiver::~RosReceiver() {
     log_file_ros_navdata_recv.close();
}

void RosReceiver::sink(char* buffer, int len, int msgId,
    bool isLast) {
    
    // assumption is that message is already assembled
    ardrone_autonomy::Navdata navMsg;
    ros::serialization::IStream istream((uint8_t*)buffer, len);
    ros::serialization::deserialize(istream, navMsg);

    navdataPub.publish(navMsg);

    timeval img_time;
    gettimeofday(&img_time, NULL);        
    double tt = GET_TDIFF(last_img_time, img_time);
    
    ros::Time thisTime = ros::Time::now();
    ros::Duration diff = thisTime - navMsg.header.stamp;
    time_spent += diff.toSec();
    frame_num++;

    if (frame_num >= 200) {
    	int offset = 0;
    	nodeHandle.getParam("/offset", offset);

        cerr << "Navdata frame rate, avg. delay: " << frame_num <<
            ", " << std::fixed << time_spent / frame_num << endl;
    	log_file_ros_navdata_recv << std::fixed << (time_spent / frame_num) - offset << " " << std::endl;
        
        frame_num = 0;
        last_img_time = img_time;
        time_spent = 0;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "ddsNavdataReceiver");

    RosReceiver rr;
    DdsConnection conn(&rr, 200, "ros-navdata", 1024*1024);
    return conn.loop(-1);
}

