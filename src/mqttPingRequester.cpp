/*******************************************************************************
  Edited by: 2016/2017 Harshit Sureka <harshit.sureka@gmail.com>
 *
 * A mqtt class to receive image packets sent by the ardrone_autonomy through
 * the mqtt bridge
 *******************************************************************************/

#include "ros/ros.h"
#include<fstream>
#include "ros/serialization.h"
#include "std_msgs/Float32MultiArray.h"
#include <mosquittopp.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include "boost/thread.hpp"

std::string CLIENTID("mqttPingRequester");

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

    ros::Publisher bridgePub_;

  public:

    //The constructor
    mqtt_bridge(const char *id, const char *host, int port, ros::NodeHandle nh);

    //The Destructor
    ~mqtt_bridge();

    boost::posix_time::ptime lastP500PosixTime;
    boost::posix_time::ptime lastP20000PosixTime;
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
    void handlePingResponse(const struct mosquitto_message *message);

    void mqttPingFunction();

    float lastp500;
    float lastp20000;
    std::vector<uint8_t> dummy20000;

    std::vector<uint8_t> dummy500;

    std::ofstream wifiPing500;
    std::ofstream wifiPing20000;
    std::ofstream mqttPing500;
    std::ofstream mqttPing20000;
};


void mqtt_bridge::mqttPingFunction()
{
  while(ros::ok())
  {

    lastP500PosixTime	= boost::posix_time::microsec_clock::local_time();
    publish(NULL, "/mqtt/ping/request", dummy500.size(), dummy500.data());


    lastP20000PosixTime = boost::posix_time::microsec_clock::local_time();
    publish(NULL, "/mqtt/ping/request", dummy20000.size(), dummy20000.data());

    sleep(1);
  }
}


//Initialize the publishers that send the message over ROS topics. 
//This is called in the constructor.
void mqtt_bridge::initPublishers()
{
  //The publisher for ROS image messages on tum_ardrone/image
  //  imagePub_ = it_.advertise("tum_ardrone/image", 1); 
  bridgePub_ = nh_.advertise<std_msgs::Float32MultiArray>("/mqtt_bridge/pings",1);
}


//The Constructor
//Intializes Variables, Intializes publishers, Connects the mqtt client.
mqtt_bridge::mqtt_bridge(const char *id, const char *host, int port, ros::NodeHandle nh) : 
  mosquittopp(id),
  nh_(nh),
  dummy500(500,1),
  dummy20000(20000,1)
{
  int keepalive = 60;

  //initialize the img ros publishers
  initPublishers();

  lastp500 = 0;
  lastp20000 = 0;

  //Connect this class instance to the mqtt host and port.
  connect(host, port, keepalive);

  std::stringstream filename;
  time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d_%m_%Y_%H_%M_%S",timeinfo);
    std::string str(buffer);

  filename << "log_file_wifiPing500_" << str << ".txt";
  wifiPing500.open(filename.str(), std::ofstream::out | std::ofstream::app);

  filename.str("");filename.clear();

  filename << "log_file_wifiPing20000_" << str << ".txt";
  wifiPing20000.open(filename.str(), std::ofstream::out | std::ofstream::app);
  
  filename.str("");filename.clear();
  filename << "log_file_mqttPing500_" << str << ".txt";
  mqttPing500.open(filename.str(), std::ofstream::out | std::ofstream::app);

  filename.str("");filename.clear();
  filename << "log_file_mqttPing20000_" << str << ".txt";
  mqttPing20000.open(filename.str(), std::ofstream::out | std::ofstream::app);

};

//Destructor
mqtt_bridge::~mqtt_bridge()
{
    wifiPing500.close();
    wifiPing20000.close();
    mqttPing500.close();
    mqttPing20000.close();
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
void mqtt_bridge::handlePingResponse(const struct mosquitto_message *message)
{
  boost::posix_time::ptime thisPosixTime	= boost::posix_time::microsec_clock::local_time();

  std::cout << "\npayloadlen : " << message->payloadlen << std::endl;
  float thisP500 = 0;
  float thisP20000 = 0;


  if(message->payloadlen <= 4) //this is the p500 msg
  {
    float dronePing500 = 10000.0;
    memcpy(&dronePing500, message->payload, sizeof(float));
 //   std::cout << "Recd. dronePing500: " << dronePing500 << " Adding to mqttPing" << std::endl;
    wifiPing500 << std::fixed << dronePing500 << std::endl;

    boost::posix_time::time_duration diff = thisPosixTime - lastP500PosixTime;
    thisP500 = (float)diff.total_microseconds()/1000.0;
    mqttPing500 << std::fixed << thisP500 << std::endl;
    thisP500 += dronePing500;

    lastp500 = 0.7*lastp500 + 0.3*thisP500;
  }
  else
  {
    float dronePing20000 = 10000.0;
    memcpy(&dronePing20000, message->payload, sizeof(float));
 //   std::cout << "Recd. dronePing20000: " << dronePing20000 << " Adding to mqttPing" << std::endl;

    boost::posix_time::time_duration diff = thisPosixTime - lastP20000PosixTime;
    thisP20000 = (float)diff.total_microseconds()/1000.0;
    mqttPing20000 << std::fixed << thisP20000 << std::endl;
    thisP20000 += dronePing20000;

    wifiPing20000 << std::fixed << dronePing20000 << std::endl;

    lastp20000 = 0.7*lastp20000 + 0.3*thisP20000;

  }

  std_msgs::Float32MultiArray msg;
  msg.data.clear();
  msg.data.push_back(lastp500);
  msg.data.push_back(lastp20000);
  bridgePub_.publish(msg);

  //std::cout << "this500 20000: " << thisP500 << " " << thisP20000 << std::endl;
//  std::cout << "new total ping 500 20000: " << msg.data[0] << " " << msg.data[1] << std::endl;
}

//When we receive a mqtt message, this callback is called. It just calls the responsible function
//depending on the topic of the mqtt message that was received.
void mqtt_bridge::on_message(const struct mosquitto_message *message)
{
  if(!strcmp(message->topic, "/mqtt/ping/response"))
  {
    handlePingResponse(message);
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
  ros::init(argc, argv, "mqttPingRequester");
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

  mqttBridge->subscribe(NULL, "/mqtt/ping/response");

  boost::thread _mqttPingThread(&mqtt_bridge::mqttPingFunction, mqttBridge);

  int rc;

  //Now we have set everything up. We just need to loop around and act as the Bridge between ROS and MQTT.

  while(ros::ok()){

    //Pump all ROS callbacks. This function pushes all messages on the ROS subscribed topics and calls the appropriate callback
    //which were defined during the subscribe call.
    // ros::spinOnce();
    rc = mqttBridge->loop();

    //Pump all MQTT callbacks. This function pushes all messages on the MQTT Subscribed topics and calls the message_callback 
    //function defined for the mosquitto instance. The callback function handles different topics internally.

    if(rc){
      mqttBridge->reconnect();
    }

  }
  _mqttPingThread.join();
  ROS_INFO("Disconnecting MQTT....\n");

  //Cleanup the Connection
  mqttBridge->disconnect();

  //Cleanup the Mosquitto Library.
  mqttBridge->lib_cleanup();

  return 0;
}

