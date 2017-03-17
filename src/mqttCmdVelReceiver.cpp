/*******************************************************************************
   Edited by: 2016 Harshit Sureka <harshit.sureka@gmail.com>
 *
 * Receiver for CmdVel, Takeoff, Reset and Land messages sent over mqtt
 
 *******************************************************************************/

#include "ros/ros.h"
#include "ros/serialization.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include <mosquittopp.h>


/////////////////////////////////////////////////////////////////////////////

std::string CLIENTID("mqttCmdVelReceiver");

//Extend class mosquittopp from the /usr/include/mosquittopp.h file 
//This class provides all the basic functions for mqtt and 
//virtual callback functions that are implemented
class mqtt_bridge : public mosquittopp::mosquittopp
{
  private:
    //ros nodehandle to handle ROS Topics publishes and subscribes
    ros::NodeHandle nh_;

    ros::Publisher cmdVelPub_;
    ros::Publisher resetPub_;
    ros::Publisher landPub_;
    ros::Publisher takeoffPub_;

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

    //Set the image and navdata publishers over ROS. Called in the constructor.
    void initPublishers();

    void handleCmdVel(const struct mosquitto_message *message);

    std::string subscribedMqttTopic_land;
    std::string publishedRosTopic_land;

    std::string subscribedMqttTopic_reset;
    std::string publishedRosTopic_reset;

    std::string subscribedMqttTopic_takeoff;
    std::string publishedRosTopic_takeoff;

    std::string subscribedMqttTopic_cmdvel;
    std::string publishedRosTopic_cmdvel;
};


//Initialize the publishers that send the message over ROS topics. 
//This is called in the constructor.
void mqtt_bridge::initPublishers()
{
	cmdVelPub_ = nh_.advertise<geometry_msgs::Twist>(publishedRosTopic_cmdvel.c_str(), 1); 
  landPub_ = nh_.advertise<std_msgs::Empty>(publishedRosTopic_land.c_str(), 1);
  resetPub_ = nh_.advertise<std_msgs::Empty>(publishedRosTopic_reset.c_str(), 1);
  takeoffPub_ = nh_.advertise<std_msgs::Empty>(publishedRosTopic_takeoff.c_str(), 1);
}


//The Constructor
//Intializes Variables, Intializes publishers, Connects the mqtt client.
mqtt_bridge::mqtt_bridge(const char *id, const char *host, int port, ros::NodeHandle nh) : 
  mosquittopp(id),
  nh_(nh)
{
  int keepalive = 60;

  subscribedMqttTopic_land = "/mqtt/land";
  publishedRosTopic_land = "/ardrone/land";

  subscribedMqttTopic_reset = "/mqtt/reset";
  publishedRosTopic_reset = "/ardrone/reset";

  subscribedMqttTopic_takeoff = "/mqtt/takeoff";
  publishedRosTopic_takeoff = "/ardrone/takeoff";

  subscribedMqttTopic_cmdvel = "/mqtt/cmd_vel";
  publishedRosTopic_cmdvel = "/cmd_vel";

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

void mqtt_bridge::handleCmdVel(const struct mosquitto_message *message)
{
  geometry_msgs::Twist cmd_vel_msg;

  uint32_t file_size = message->payloadlen;
  boost::shared_array<uint8_t> ibuffer(new uint8_t[file_size]);
  ros::serialization::IStream istream(message->payload, file_size);
  ros::serialization::deserialize(istream, cmd_vel_msg);

  cmdVelPub_.publish(cmd_vel_msg);
  return;
}

//When we receive a mqtt message, this callback is called. It just calls the responsible function
//depending on the topic of the mqtt message that was received.
void mqtt_bridge::on_message(const struct mosquitto_message *message)
{
	if(!strcmp(message->topic, subscribedMqttTopic_reset.c_str()))
	{
		resetPub_.publish(std_msgs::Empty());
	}
	else if(!strcmp(message->topic, subscribedMqttTopic_land.c_str() ))
	{
		landPub_.publish(std_msgs::Empty());
	}
	else if(!strcmp(message->topic, subscribedMqttTopic_takeoff.c_str()))
	{
		takeoffPub_.publish(std_msgs::Empty());
	}
  else if(!strcmp(message->topic, subscribedMqttTopic_cmdvel.c_str()))
  {
    handleCmdVel(message);
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
  ros::init(argc, argv, "mqttCmdVelReceiver");
  ros::NodeHandle nodeHandle;
  
  std::string broker = "localhost";
  int brokerPort = 1883;
  nodeHandle.getParam("/mqttBrokerPort", brokerPort);
  ros::param::get("/mqttBroker", broker);
  
  class mqtt_bridge *mqttBridge;
  mqttBridge->lib_init();
  mqttBridge = new mqtt_bridge(CLIENTID.c_str(), broker.c_str(), brokerPort, nodeHandle);

  //Read Parameters from the launch file
  ros::param::get("/mqttCmdVelReceiver/subscribedMqttTopic_cmdvel", mqttBridge->subscribedMqttTopic_cmdvel);
  ros::param::get("/mqttCmdVelReceiver/subscribedMqttTopic_reset", mqttBridge->subscribedMqttTopic_reset);
  ros::param::get("/mqttCmdVelReceiver/subscribedMqttTopic_land", mqttBridge->subscribedMqttTopic_land);
  ros::param::get("/mqttCmdVelReceiver/subscribedMqttTopic_takeoff", mqttBridge->subscribedMqttTopic_takeoff);


  ros::param::get("/mqttCmdVelReceiver/publishedRosTopic_cmdvel", mqttBridge->publishedRosTopic_cmdvel);
  ros::param::get("/mqttCmdVelReceiver/publishedRosTopic_reset", mqttBridge->publishedRosTopic_reset);
  ros::param::get("/mqttCmdVelReceiver/publishedRosTopic_land", mqttBridge->publishedRosTopic_land);
  ros::param::get("/mqttCmdVelReceiver/publishedRosTopic_takeoff", mqttBridge->publishedRosTopic_takeoff);
  
  ROS_INFO("MQTT %s -> ROS %s, MQTT %s -> ROS %s, MQTT %s -> ROS %s, MQTT %s -> ROS %s\n", 
      mqttBridge->subscribedMqttTopic_cmdvel.c_str(),  mqttBridge->publishedRosTopic_cmdvel.c_str(),
      mqttBridge->subscribedMqttTopic_reset.c_str(), mqttBridge->publishedRosTopic_reset.c_str(),
      mqttBridge->subscribedMqttTopic_land.c_str(), mqttBridge->publishedRosTopic_land.c_str(),
      mqttBridge->subscribedMqttTopic_takeoff.c_str(), mqttBridge->publishedRosTopic_takeoff.c_str());

  mqttBridge->initPublishers();

  ROS_INFO("Connecting to %s at %d\n", broker.c_str(), brokerPort);
 	
  mqttBridge->subscribe(NULL, mqttBridge->subscribedMqttTopic_cmdvel.c_str());
 	mqttBridge->subscribe(NULL, mqttBridge->subscribedMqttTopic_reset.c_str());
 	mqttBridge->subscribe(NULL, mqttBridge->subscribedMqttTopic_land.c_str());
 	mqttBridge->subscribe(NULL, mqttBridge->subscribedMqttTopic_takeoff.c_str());

  int rc;

  //Now we have set everything up. We just need to loop around and act as the Bridge between ROS and MQTT.
  while(ros::ok()){

    //Pump all ROS callbacks. This function pushes all messages on the ROS subscribed topics and calls the appropriate callback
    //which were defined during the subscribe call.
    ros::spinOnce();

    //Pump all MQTT callbacks. This function pushes all messages on the MQTT Subscribed topics and calls the message_callback 
    //function defined for the mosquitto instance. The callback function handles different topics internally.
    rc = mqttBridge->loop();

    //If the first ping isnt acknowledged, keep pinging.
		
		if(rc){
			mqttBridge->reconnect();
    }
  }

  ROS_INFO("Disconnecting MQTT....\n");

  //Cleanup the Connection
  mqttBridge->disconnect();

  //Cleanup the Mosquitto Library.
  mqttBridge->lib_cleanup();

  return 0;
}

