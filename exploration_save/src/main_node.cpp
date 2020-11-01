#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <kesla_msg/DoneService.h>
#include <string>
#include <geometry_msgs/PointStamped.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define MAX_SIZE 1000
char inputString[MAX_SIZE];

using namespace std;


//전역 변수
ros::Publisher pubClickedPoint;
//


bool srv_callback(kesla_msg::DoneService::Request &req,
                  kesla_msg::DoneService::Response &res);
void makeTextfile(const char* myString);
void sendClickedPoint(float x, float y);
void msgCallback(const nav_msgs::Odometry::ConstPtr& msg);

bool srv_callback(kesla_msg::DoneService::Request &req,
                  kesla_msg::DoneService::Response &res)
{
  cout << req.myRequest <<endl;
  if(!req.myRequest.compare("finished")){
    res.myResponse = "finished success";
    //텍스트 파일 저장//
  }else if(!req.myRequest.compare("excuted")){
    res.myResponse = "excuted success";
    sendClickedPoint(-10,10);
    sendClickedPoint(10,10);
    sendClickedPoint(10,-10);
    sendClickedPoint(-10,-10);
    /*
    sendClickedPoint(-10,10);
    sendClickedPoint(0,0);
    */
  }else{
    res.myResponse = "fail";
  }
  return true;
}

void makeTextfile(const char* myString){
  //myString의 변수를 file의 한 줄에 추가//
  ofstream outFile("odom_start.txt");
  outFile << myString << endl;

  outFile.close();
  //
}

void sendClickedPoint(float x, float y){
  geometry_msgs::PointStamped msg;
  msg.header.frame_id = "map";
  msg.point.x = x; msg.point.y = y; msg.point.z = 0;
  pubClickedPoint.publish(msg);
  cout<< "보내는중" << endl;
}

void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){
//nav_msgs 토픽의 Odometry 메세지를 받음.
  std::cout << "x:" << msg->pose.pose.position.x << std::endl;
  std::cout << "y:" << msg->pose.pose.position.y << std::endl;
  std::cout << "z:" << msg->pose.pose.position.z << std::endl;
  stringstream ss;
  ss << "odom_start:"<< msg->pose.pose.position.x << "," << msg->pose.pose.position.y << std::endl;

  makeTextfile(ss.str().c_str());
}

int main(int argc, char** argv){

  ros::init(argc, argv, "exploration_save");

  ros::NodeHandle n;
  //service
  ros::ServiceServer clientDone = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  ros::ServiceServer clientExcuted = n.advertiseService("explore_client/sendExplorExcuted",srv_callback);
  //subscriber
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  //Publisher
  pubClickedPoint = n.advertise<geometry_msgs::PointStamped>("/clicked_point",10);

  ros::Rate rate(20.0);
  ros::Time beforeTime;

  while(beforeTime == ros::Time(0)){ // 확실한 beforeTime을 받아오기 위해
    beforeTime = ros::Time::now();
  }
  while(n.ok()){
    if(ros::Time::now() - beforeTime > ros::Duration(5)){ // 5초마다 동작중임을 알림
      cout << "exploration_save 동작중" << endl;
      beforeTime = ros::Time::now();
    }

    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
