#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <kesla_msg/DoneService.h>
#include <string>

using namespace std;
bool srv_callback(kesla_msg::DoneService::Request &req,
                  kesla_msg::DoneService::Response &res)
{
  cout << req.myRequest <<endl;
  if(req.myRequest.compare("finished") == 0){
    res.myResponse = "success";
  }else{
    res.myResponse = "fail";
  }
  return true;
}

void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){
  std::cout << "x:" << msg->pose.pose.position.x << std::endl;

}


int main(int argc, char** argv){

  ros::init(argc, argv, "exploration_save");

  ros::NodeHandle n;
  ros::ServiceServer client = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  ROS_INFO("Be ready");
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  ros::spin();

  return 0;

}
