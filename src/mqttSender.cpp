/*******************************************************************************
   Edited by: Dec 2016 Harshit Sureka <harshit.sureka@gmail.com>
 *
 * The mqtt_bridge node for sending images and navdata from the ardrone_autonomy
 *
 *******************************************************************************/

#include "ros/ros.h"
#include "ros/serialization.h"
#include <mosquittopp.h>
#include <image_transport/image_transport.h>
#include <ardrone_autonomy/Navdata.h>
#include <fstream>

/////////////////////////////////////////////////////////////////////////////

std::string CLIENTID("mqttSender");

//Extend class mosquittopp from the /usr/include/mosquittopp.h file 
//This class provides all the basic functions for mqtt and 
//virtual callback functions that are implemented
class MQTTSender : public mosquittopp::mosquittopp
{
  private:
    //ros nodehandle to handle ROS Topics publishes and subscribes
    ros::NodeHandle nh_;

  public:
    //The constructor

    ros::Subscriber vid_sub;
    ros::Subscriber navdata_sub;

    MQTTSender(const char *id, const char *host, int port, ros::NodeHandle nh);

    //The Destructor
    ~MQTTSender();

    //Callback for when the mqtt client is connected
    void on_connect(int rc);

    //Callback for when the mqtt client receives a message on a subscribed topic
    void on_message(const struct mosquitto_message *message);

    //Callback for when the mqtt message succeeds in subscribing to a topic
    void on_subscribe(int mid, int qos_count, const int *granted_qos);

    //This is a callback for receiving a navdata msg over ROS. It is then sent over MQTT.
    void navdataMessageCallback(const ardrone_autonomy::NavdataConstPtr msg);

    //This is a callback for receiving an image msg over ROS. It is then sent over MQTT.
    void imageMessageCallback(const sensor_msgs::ImageConstPtr msg);

    std::string subscribedRosTopic_image;
    std::string publishedMqttTopic_image;

    std::string subscribedRosTopic_navdata;
    std::string publishedMqttTopic_navdata;

    std::ofstream log_file_ros_image_recv;
    ros::Time last_ros_image_recv;

    std::ofstream log_file_mqtt_image_send;
    ros::Time last_mqtt_image_send;

    int navdataPublishFreq;
    int imagePublishFreq;
};

//The Constructor
//Intializes Variables, Intializes publishers, Connects the mqtt client.
MQTTSender::MQTTSender(const char *id, const char *host, int port, ros::NodeHandle nh) : 
  mosquittopp(id),
  nh_(nh)
{
  int keepalive = 60;

  subscribedRosTopic_image = "/ardrone/image_raw";
  publishedMqttTopic_image = "/mqtt/image";

  subscribedRosTopic_navdata = "/ardrone/navdata";
  publishedMqttTopic_navdata = "/mqtt/navdata";
  
  imagePublishFreq = 30;
  navdataPublishFreq = 200;

  ros::param::get("/mqttSender/ImagePublishFreq", imagePublishFreq);
  std::cout << "set ImagePublishFreq to " << imagePublishFreq << "Hz"<< std::endl;
  
  ros::param::get("/mqttSender/NavdataPublishFreq", navdataPublishFreq);
  std::cout << "set NavdataPublishFreq to " << navdataPublishFreq << "Hz"<< std::endl;

  //Connect this class instance to the mqtt host and port.
  connect(host, port, keepalive);

  ros::param::get("/mqttSender/subscribedRosTopic_navdata", subscribedRosTopic_navdata);
  ros::param::get("/mqttSender/subscribedRosTopic_image", subscribedRosTopic_image);
  
  ros::param::get("/mqttSender/publishedMqttTopic_navdata", publishedMqttTopic_navdata);
  ros::param::get("/mqttSender/publishedMqttTopic_image", publishedMqttTopic_image);

	vid_sub = nh_.subscribe(subscribedRosTopic_image.c_str(), 10, &MQTTSender::imageMessageCallback, this);
  navdata_sub = nh_.subscribe(subscribedRosTopic_navdata.c_str(), 10, &MQTTSender::navdataMessageCallback, this);

  log_file_ros_image_recv.open("log_file_ros_image_recv.txt",std::ofstream::out | std::ofstream::app);
  last_ros_image_recv = ros::Time::now();

  log_file_mqtt_image_send.open("log_file_mqtt_image_send.txt",std::ofstream::out | std::ofstream::app);
  last_mqtt_image_send = ros::Time::now();
};

//Destructor
MQTTSender::~MQTTSender()
{
  log_file_ros_image_recv.close();
  log_file_mqtt_image_send.close();
}

//Callback when the mqtt client successfully connects. rc = 0 means successful connection.
void MQTTSender::on_connect(int rc)
{
  ROS_INFO("Connected with code %d.\n", rc);
}

void MQTTSender::on_message(const struct mosquitto_message *message)
{
}

//Callback when the mosquitto library successfully subscribes to a topic
void MQTTSender::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
  ROS_INFO("Subscription succeeded.\n");
}

void MQTTSender::navdataMessageCallback(const ardrone_autonomy::NavdataConstPtr msg)
{
  uint32_t serial_size = ros::serialization::serializationLength(*msg);
  boost::shared_array<uint8_t> obuffer(new uint8_t[serial_size]);

  ros::serialization::OStream ostream(obuffer.get(), serial_size);
  ros::serialization::serialize(ostream, *msg);
  publish(NULL, publishedMqttTopic_navdata.c_str(), serial_size, obuffer.get());
 
  return;
}
void MQTTSender::imageMessageCallback(const sensor_msgs::ImageConstPtr msg)
{
  
  log_file_ros_image_recv << std::fixed << (ros::Time::now() - last_ros_image_recv).toSec() << ", " << msg->header.seq << ", " << (msg->header.stamp).toSec() << std::endl;
  last_ros_image_recv = ros::Time::now();

//  sensor_msgs::Image new_msg = *msg;
//  new_msg.header.stamp = ros::Time::now();

  uint32_t serial_size = ros::serialization::serializationLength(*msg);
  boost::shared_array<uint8_t> obuffer(new uint8_t[serial_size]);

  ros::serialization::OStream ostream(obuffer.get(), serial_size);
  ros::serialization::serialize(ostream, *msg);

	publish(NULL, publishedMqttTopic_image.c_str(), serial_size, obuffer.get());
  log_file_mqtt_image_send << (ros::Time::now() - last_mqtt_image_send).toSec() << std::endl;
  last_mqtt_image_send = ros::Time::now();

  return;
}

int main(int argc, char **argv)
{
  //Start with a new random client ID each time, so that prev messages aren't a hassle.
  srand(time(NULL));
  CLIENTID += std::to_string(rand());

  //Mandatory ROS INIT call for this file to be registered as a ROS NODE. 
  ros::init(argc, argv, "mqttSender");
  ros::NodeHandle nodeHandle;

  std::string broker = "localhost";
  int brokerPort = 1883;
  ros::param::get("/mqttBroker", broker);
  nodeHandle.getParam("/mqttBrokerPort", brokerPort);

  ROS_INFO("Connecting to %s at %d\n", broker.c_str(), brokerPort);
  
  //Initialize the mqttSender class instance
  class MQTTSender *mqttSender;

  mqttSender->lib_init();

  mqttSender = new MQTTSender(CLIENTID.c_str(), broker.c_str(), brokerPort, nodeHandle);
  ROS_INFO("mqttSender initialized..\n");
  
 
  int rc;

  ros::Rate rate(mqttSender->imagePublishFreq);

  //Now we have set everything up. We just need to loop around and act as the Bridge between ROS and MQTT.
  while(ros::ok()){

    //Pump all ROS callbacks. This function pushes all messages on the ROS subscribed topics and calls the appropriate callback
    //which were defined during the subscribe call.
    ros::spinOnce();

    //Pump all MQTT callbacks. This function pushes all messages on the MQTT Subscribed topics and calls the message_callback 
    //function defined for the mosquitto instance. The callback function handles different topics internally.
//    rc = mqttSender->loop();

    //If the mqtt connection is giving any troubles. Try to reconnect.
//    if(rc){
//      mqttSender->reconnect();
//    }
    rate.sleep();
  }

  ROS_INFO("Disconnecting MQTT....\n");

  //Cleanup the Connection
  mqttSender->disconnect();

  //Cleanup the Mosquitto Library.
  mqttSender->lib_cleanup();

  return 0;
}

