#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <kesla_msg/DoneService.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

enum MISSION_MODE{
  ODOM_START,
  ODOM_STAY,
  ODOM_FINISH,
}

#define MAX_SIZE 1000
char inputString[MAX_SIZE];


using namespace std;
bool srv_callback(kesla_msg::DoneService::Request &req,
                  kesla_msg::DoneService::Response &res)
{
  cout << req.myRequest <<endl;
  if(req.myRequest.compare("finished") == 0){
    //frontier_exploration 끝났을때 finished survice가 오면 success survice 보냄//]]
    res.myResponse = "success";
  }else{
    res.myResponse = "fail";
  }
  return true;
}

void makeTextfile(const char* myString){
  //myString의 변수를 file의 한 줄에 추가//
  ofstream outFile("../catkin_ws/src/kesla_project/exploration_save/txtfile/odom_start.txt");
  outFile << myString << endl;

  outFile.close();
}

void printOdom(){

  std::cout << "x:" << msg->pose.pose.position.x << std::endl;
  std::cout << "y:" << msg->pose.pose.position.y << std::endl;
  std::cout << "z:" << msg->pose.pose.position.z << std::endl;
}


void msgCallback(const nav_msgs::Odometry::ConstPtr& msg, int* condition){
//nav_msgs 토픽의 Odometry 메세지를 받음.
  
  printOdom();
  stringstream ss;
  ss << "odom_start:" << msg->pose.pose.position.x << "," << msg->pose.pose.position.y << std::endl;
  ss << "quaternion_finish:" << msg->pose.pose.orientation.w << "," << msg->pose.pose.orientation.x << "," << msg->pose.pose.orientation.y << "," << msg->pose.pose.orientation.z << std::endl;

  makeTextfile(ss.str().c_str());
}

int main(int argc, char** argv){

  ros::init(argc, argv, "exploration_save");

  ros::NodeHandle n;
  ros::ServiceServer client = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  ros::spin();
  //

  return 0;
}
