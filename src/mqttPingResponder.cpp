/*******************************************************************************
  Edited by: 2016/2017 Harshit Sureka <harshit.sureka@gmail.com>
 *
 * A mqtt class to receive image packets sent by the ardrone_autonomy through
 * the mqtt bridge
 *******************************************************************************/

#include "ros/ros.h"
#include "ros/serialization.h"
#include "boost/thread.hpp"
#include <mosquittopp.h>

std::string CLIENTID("mqttPingResponder");

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

    float p500;
    float  p20000;
    char pingCommand500[100];
    char pingCommand20000[100];
    char line1[200];
    char line2[200];

    void PingFunction();
    float parsePingResult(std::string s);
    uint8_t responseBuf[5];
};

float mqtt_bridge::parsePingResult(std::string s)
{
  // 20008 bytes from localhost (127.0.0.1): icmp_req=1 ttl=64 time=0.075 ms
  int pos = s.find("time=");
  int found = 0;
  float ms;
  if(pos != std::string::npos)
    found = sscanf(s.substr(pos).c_str(),"time=%f",&ms);

  if(found == 1 && pos != std::string::npos)
    return ms;
  else
    return 10000;
}
void mqtt_bridge::PingFunction()
{
  std::cout << "Starting PING Thread" << std::endl;

  sprintf(pingCommand20000,"ping -c 1 -s 20000 -w 1 192.168.1.1");
  sprintf(pingCommand500,"ping -c 1 -s 500 -w 1 192.168.1.1");
//  	sprintf(pingCommand20000,"ping -c 1 -s 20000 -w 1 127.0.0.1");
//  	sprintf(pingCommand500,"ping -c 1 -s 500 -w 1 127.0.0.1");
  ros::Rate r(2.0);
  FILE *p;

  while(ros::ok())
  {
    // ping twice, with a sleep in between
    p = popen(pingCommand500,"r");
    fgets(line1, 200, p);
    fgets(line1, 200, p);
    pclose(p);

    // sleep 1s
    r.sleep();
    if(!ros::ok()) break;
    r.sleep();
    if(!ros::ok()) break;

    p = popen(pingCommand20000,"r");
    fgets(line2, 200, p);
    fgets(line2, 200, p);
    pclose(p);

    // parse results which should be in line1 and line2
    float res500 = parsePingResult(line1);
    float res20000 = parsePingResult(line2);

    std::cout << "new ping values: 500->" << res500 << " 20000->" << res20000 << std::endl;

    // clip between 10 and 1000.
    res500 = std::min(1000.0f,std::max(10.0f,res500));
    res20000 = std::min(1000.0f,std::max(10.0f,res20000));

    // update
    p500 = 0.7 * p500 + 0.3 * res500;
    p20000 = 0.7 * p20000 + 0.3 * res20000;

    // send
    snprintf(line1,200,"pings %d %d", (int)p500, (int)p20000);
    std::cout << line1 << std::endl;
    //if(rosThread != NULL) rosThread->publishCommand(line1);
    //if(gui != NULL) gui->setPings((int)p500, (int)p20000);

    // sleep 1s
    r.sleep();
    if(!ros::ok()) break;
    r.sleep();
    if(!ros::ok()) break;
  }

  std::cout << "Exiting PING Thread" << std::endl;
}

//Initialize the publishers that send the message over ROS topics. 
//This is called in the constructor.
void mqtt_bridge::initPublishers()
{
  //The publisher for ROS image messages on tum_ardrone/image
  //  imagePub_ = it_.advertise("tum_ardrone/image", 1); 
}


//The Constructor
//Intializes Variables, Intializes publishers, Connects the mqtt client.
mqtt_bridge::mqtt_bridge(const char *id, const char *host, int port, ros::NodeHandle nh) : 
  mosquittopp(id),
  nh_(nh)
{
  int keepalive = 60;
  p500 = 25;
  p20000 = 50;

  //initialize the img ros publishers
  initPublishers();

  //Connect this class instance to the mqtt host and port.
  connect(host, port, keepalive);
};

//Destructor
mqtt_bridge::~mqtt_bridge()
{
}

//Callback when the mqtt client successfully connects. rc = 0 means successful connection.
void mqtt_bridge::on_connect(int rc)
{
  ROS_INFO("Connected with code %d.\n", rc);
}

//When we receive a mqtt message, this callback is called. It just calls the responsible function
//depending on the topic of the mqtt message that was received.
void mqtt_bridge::on_message(const struct mosquitto_message *message)
{
  if(!strcmp(message->topic, "/mqtt/ping/request"))
  {
    if(message->payloadlen < 10000)//this is the p500 message
    {
      memcpy(responseBuf, &p500, sizeof(float));
      publish(NULL, "/mqtt/ping/response", sizeof(float), responseBuf);
    }
    else //this is the p20000 msg
    {
      memcpy(responseBuf, &p20000, sizeof(float));
      publish(NULL, "/mqtt/ping/response", sizeof(float) + 1, responseBuf);
    }

    //clear the buffer
    for(int i = 0; i < 5; i++)
      responseBuf[i] = 0;
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
  ros::init(argc, argv, "mqttPingResponder");
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

  mqttBridge->subscribe(NULL, "/mqtt/ping/request");

  boost::thread _PingThread(&mqtt_bridge::PingFunction, mqttBridge);

  int rc;

  //Now we have set everything up. We just need to loop around and act as the Bridge between ROS and MQTT.
  while(ros::ok()){

    //Pump all MQTT callbacks. This function pushes all messages on the MQTT Subscribed topics and calls the message_callback 
    //function defined for the mosquitto instance. The callback function handles different topics internally.
    rc = mqttBridge->loop();

    if(rc){
      mqttBridge->reconnect();
    }
  }

  ROS_INFO("Disconnecting MQTT....\n");
  _PingThread.join();
  //Cleanup the Connection
  mqttBridge->disconnect();

  //Cleanup the Mosquitto Library.
  mqttBridge->lib_cleanup();

  return 0;
}

