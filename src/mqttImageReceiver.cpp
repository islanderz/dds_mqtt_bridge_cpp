/*******************************************************************************
   Edited by: 2016/2017 Harshit Sureka <harshit.sureka@gmail.com>
 *
 * A mqtt class to receive image packets sent by the ardrone_autonomy through
 * the mqtt bridge
 *******************************************************************************/

#include "ros/ros.h"
#include "ros/serialization.h"
#include <mosquittopp.h>
#include <image_transport/image_transport.h>
#include <fstream>

std::string CLIENTID("mqttImageReceiver");

/*
 * Extend class mosquittopp from the /usr/include/mosquittopp.h file 
 * This class provides all the basic functions for mqtt and 
 * virtual callback functions that are implemented
 */
class mqtt_bridge : public mosquittopp::mosquittopp
{
  private:
    //ros nodehandle to handle ROS Topics publishes and subscribes
    ros::NodeHandle nh_;

    //Message type for transporting images in ROS
    image_transport::ImageTransport it_;

    //The publisher for images on ROS topic
    image_transport::Publisher imagePub_;

  public:

    //The constructor
    mqtt_bridge(const char *id, const char *host, int port, ros::NodeHandle nh);

    //The Destructor
    ~mqtt_bridge();

    //Callback for when the mqtt client is connected
    void on_connect(int rc);

    //Callback for when the mqtt client receives a message on a subscribed topic
    void on_message(const struct mosquitto_message *message);

    //Callback for when the mqtt message succeeds in subscribing to a topic
    void on_subscribe(int mid, int qos_count, const int *granted_qos);

    //Set the image publisher over ROS. Called in the constructor.
    void initPublishers();

    //Callback reidrects here when a uncompressedImage message is received over MQTT. The timestamp is extracted and then 
    //the file is packaged into an imageTransport message and sent as a ROS topic.
    void handleUncompressedImage(const struct mosquitto_message *message);
    std::string publishedRosTopic_image;
    std::string subscribedMqttTopic_image;
    double imgDelayAverage;
    int imgCount;

    std::ofstream log_file_ros_image_recv;
    ros::Time last_ros_image_recv;
};


//Initialize the publishers that send the message over ROS topics. 
//This is called in the constructor.
void mqtt_bridge::initPublishers()
{
  //The publisher for ROS image messages on tum_ardrone/image
  imagePub_ = it_.advertise(publishedRosTopic_image.c_str(), 1); 
}


//The Constructor
//Intializes Variables, Intializes publishers, Connects the mqtt client.
mqtt_bridge::mqtt_bridge(const char *id, const char *host, int port, ros::NodeHandle nh) : 
  mosquittopp(id),
  nh_(nh), 
  it_(nh)
{
  int keepalive = 60;
    
  publishedRosTopic_image = "/ardrone/image_raw";
  subscribedMqttTopic_image = "/mqtt/image";
  imgDelayAverage = 0.0;
  imgCount = 0;

  //Connect this class instance to the mqtt host and port.
  connect(host, port, keepalive);

  std::stringstream ss;
  last_ros_image_recv = ros::Time::now();

  time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d_%m_%Y_%H_%M_%S",timeinfo);
    std::string str(buffer);

  ss << "log_file_image_av_delays_" << str << ".txt";
  log_file_ros_image_recv.open(ss.str(), std::ofstream::out | std::ofstream::app);

};

//Destructor
mqtt_bridge::~mqtt_bridge()
{
	log_file_ros_image_recv.close();
}

//Callback when the mqtt client successfully connects. rc = 0 means successful connection.
void mqtt_bridge::on_connect(int rc)
{
  ROS_INFO("Connected with code %d.\n", rc);
}

/* 
 * This is the function which handles the incoming images over MQTT.
 * The images are prefixed by a timestamp which is extracted and then
 * the image data is packed in a ROS Image_Transport message and published
 * over the ROS Topic /ardrone/image_raw
 */
void mqtt_bridge::handleUncompressedImage(const struct mosquitto_message *message)
{



  //Get the current time
  sensor_msgs::Image image_msg;

  uint32_t file_size = message->payloadlen;
  boost::shared_array<uint8_t> ibuffer(new uint8_t[file_size]);
  ros::serialization::IStream istream(message->payload, file_size);
  ros::serialization::deserialize(istream, image_msg);

  imagePub_.publish(image_msg);
  ros::Time thisTime = ros::Time::now();
  ros::Duration diff = thisTime - image_msg.header.stamp;
  imgCount += 1;
  imgDelayAverage += diff.toSec();



  if(imgCount >= 30)
  {
    std::cout << std::fixed << "Average delay of last 30 image messages: " << imgDelayAverage/imgCount << std::endl;

    log_file_ros_image_recv << std::fixed << imgDelayAverage/imgCount << " " << std::endl;
    last_ros_image_recv = ros::Time::now();

    imgCount = 0;
    imgDelayAverage = 0.0;
  }

  return;
}

//When we receive a mqtt message, this callback is called. It just calls the responsible function
//depending on the topic of the mqtt message that was received.
void mqtt_bridge::on_message(const struct mosquitto_message *message)
{
	if(!strcmp(message->topic, subscribedMqttTopic_image.c_str()))
	{
		handleUncompressedImage(message);
	}
}

//Callback when the mosquitto library successfully subscribes to a topic
void mqtt_bridge::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
  ROS_INFO("Subscription succeeded.\n");
}

int main(int argc, char **argv)
{
  //Start with a new random client ID each time, so that prev messages aren't a hassle.
  srand(time(NULL));
  CLIENTID += std::to_string(rand());

  //Mandatory ROS INIT call for this file to be registered as a ROS NODE. 
  ros::init(argc, argv, "mqttImageReceiver");
  ros::NodeHandle nodeHandle;

  //Initialize different variables that are to be read from the parameter file.
  std::string broker = "localhost";
  int brokerPort = 1883;

  nodeHandle.getParam("/mqttBrokerPort", brokerPort);
  ros::param::get("/mqttBroker", broker);

  ROS_INFO("Connecting to %s at %d port\n",broker.c_str(), brokerPort);

  //Initialize the mqttBridge class instance
  class mqtt_bridge *mqttBridge;

  mqttBridge->lib_init();

  mqttBridge = new mqtt_bridge(CLIENTID.c_str(), broker.c_str(), brokerPort, nodeHandle);
  ROS_INFO("mqttBridge initialized..\n");

  ros::param::get("/mqttImageReceiver/subscribedMqttTopic_image", mqttBridge->subscribedMqttTopic_image);
  ros::param::get("/mqttImageReceiver/publishedRosTopic_image", mqttBridge->publishedRosTopic_image);

  mqttBridge->initPublishers();

	mqttBridge->subscribe(NULL, mqttBridge->subscribedMqttTopic_image.c_str());

  int rc;

  int loop_rate = 40;
  ros::param::get("/mqttImageReceiver/loop_rate",loop_rate);
  ros::Rate rate(loop_rate);

  ROS_INFO("Running with loop_rate: %d\n",loop_rate);

  //Now we have set everything up. We just need to loop around and act as the Bridge between ROS and MQTT.
  while(ros::ok()){

    //Pump all ROS callbacks. This function pushes all messages on the ROS subscribed topics and calls the appropriate callback
    //which were defined during the subscribe call.
    ros::spinOnce();

    //Pump all MQTT callbacks. This function pushes all messages on the MQTT Subscribed topics and calls the message_callback 
    //function defined for the mosquitto instance. The callback function handles different topics internally.
    rc = mqttBridge->loop();

		if(rc){
			mqttBridge->reconnect();
    }

    rate.sleep();
  }

  ROS_INFO("Disconnecting MQTT....\n");

  //Cleanup the Connection
  mqttBridge->disconnect();

  //Cleanup the Mosquitto Library.
  mqttBridge->lib_cleanup();

  return 0;
}

