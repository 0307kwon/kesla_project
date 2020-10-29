#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <kesla_msg/DoneService.h>
#include <string>
#include <geometry_msgs/PointStamped.h>
using namespace std;
bool srv_callback(kesla_msg::DoneService::Request &req,
                  kesla_msg::DoneService::Response &res)
{
  cout << req.myRequest <<endl;
  if(req.myRequest.compare("finished") == 0){
    res.myResponse = "success";
    //텍스트 파일 저장//
  }else{
    res.myResponse = "fail";
  }
  return true;
}

void makeTextfile(const char* myString){
  // myString의 변수를 파일의 한줄에 추가//
}

void sendClickedPoint(float x, float y,ros::Publisher& pubClickedPoint){
  geometry_msgs::PointStamped msg;
  msg.header.frame_id = "map";
  msg.point.x = x; msg.point.y = y; msg.point.z = 0;
  pubClickedPoint.publish(msg);
  cout<< "보내는중" << endl;
}

void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){
  //성광//
}
int main(int argc, char** argv){

  ros::init(argc, argv, "exploration_save");

  ros::NodeHandle n;
  //service
  ros::ServiceServer client = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  //subscriber
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  //Publisher
  ros::Publisher pubClickedPoint = n.advertise<geometry_msgs::PointStamped>("/clicked_point",10);

  ros::Rate rate(20.0);
  ros::Time beforeTime;
  bool init = false;

  while(beforeTime == ros::Time(0)){ // 확실한 beforeTime을 받아오기 위해
    beforeTime = ros::Time::now();
  }
  while(n.ok()){
    if(ros::Time::now() - beforeTime > ros::Duration(5) && init == false){
      sendClickedPoint(-10,10,pubClickedPoint);
      sendClickedPoint(10,10,pubClickedPoint);
      sendClickedPoint(10,-10,pubClickedPoint);
      sendClickedPoint(-10,-10,pubClickedPoint);
      sendClickedPoint(-10,10,pubClickedPoint);
      sendClickedPoint(0,0,pubClickedPoint);
      beforeTime = ros::Time::now();
      init = true;
    }

    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
