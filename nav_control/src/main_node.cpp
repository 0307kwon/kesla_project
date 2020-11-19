#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <kesla_msg/DoneService.h>
#include <string>
#include <geometry_msgs/PointStamped.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


int main(int argc, char** argv){

  ros::init(argc, argv, "nav_control");

  ros::NodeHandle n;

  /*
  //service
  ros::ServiceServer clientDone = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  ros::ServiceServer clientExcuted = n.advertiseService("explore_server/sendExplorExcuted",srv_callback);
  //subscriber
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  */

  //Publisher
  ros::Publisher pubClickedPoint = n.advertise<geometry_msgs::PointStamped>("/clicked_point",10);

  ros::Rate rate(20.0);
  ros::Time beforeTime;

  int duration_time = 5;

  while(beforeTime == ros::Time(0)){ // 확실한 beforeTime을 받아오기 위해
    beforeTime = ros::Time::now();
  }
  while(n.ok()){
    if(ros::Time::now() - beforeTime > ros::Duration(duration_time)){ // 5초마다 동작중임을 알림
      cout << "nav_control 동작중" << endl;
      beforeTime = ros::Time::now();
    }

    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
