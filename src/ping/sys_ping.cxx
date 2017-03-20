#include "ros/ros.h"
#include <iostream>

float parsePingResult(std::string s)
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

void ping_function()
{
    float p500;
    float  p20000;
    char pingCommand500[100];
    char pingCommand20000[100];
    char line1[200];
    char line2[200];


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


int main(int argc, char **argv) {
    ros::init(argc, argv, "mqttPingResponder");
    ros::NodeHandle nodeHandle;

    ping_function();
    return 0;
}
