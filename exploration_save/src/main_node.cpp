#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){
  std::cout << "x:" << msg->pose.pose.position.x << std::endl;

}

int main(int argc, char** argv){

  ros::init(argc, argv, "exploration_save");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);

  ros::spinOnce();

}
