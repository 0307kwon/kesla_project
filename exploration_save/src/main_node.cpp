#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <kesla_msg/DoneService.h>
#include <string>
<<<<<<< HEAD
=======


>>>>>>> 3399cdf177c0c1dc7dfe1a8f30061bc3ccd4b15a
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

<<<<<<< HEAD
void makeTextfile(const char* myString){
  // myString의 변수를 파일의 한줄에 추가//
}
=======
void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){

  std::cout << "x:" << msg->pose.pose.position.x << std::endl;
  std::cout << "y:" << msg->pose.pose.position.y << std::endl;
  std::cout << "z:" << msg->pose.pose.position.z << std::endl;
>>>>>>> 3399cdf177c0c1dc7dfe1a8f30061bc3ccd4b15a

void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){
  //성광//
}
int main(int argc, char** argv){
  ros::init(argc, argv, "exploration_save");

  ros::NodeHandle n;
  ros::ServiceServer client = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  ros::spin();
  return 0;
}
