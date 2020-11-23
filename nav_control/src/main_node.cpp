#include <ros/ros.h>
#include <string>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <move_base_msgs/MoveBaseActionGoal.h>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

string txtSave_path_;
bool isFirst1 = false;
ros::Publisher *pubInitialPose_ptr;

geometry_msgs::PoseWithCovarianceStamped initPose_msg;
move_base_msgs::MoveBaseActionGoal goal_msg;
void readLogfile(){
  if(freopen(txtSave_path_.c_str(),"r",stdin) == NULL){
		ROS_WARN_STREAM("cannot find file location parameter");
		return;
	}

	string t;
  cin >> t;
  cout << t << endl;
  goal_msg.header.stamp = ros::Time::now();
  goal_msg.goal.target_pose.header.stamp = ros::Time::now();
  goal_msg.goal.target_pose.header.frame_id = "map";
  cin >> goal_msg.goal.target_pose.pose.position.x;
  cin >> goal_msg.goal.target_pose.pose.position.y;
  goal_msg.goal.target_pose.pose.position.z = 0;
  cin >> t;
  cin >> goal_msg.goal.target_pose.pose.orientation.x;
  cin >> goal_msg.goal.target_pose.pose.orientation.y;
  cin >> goal_msg.goal.target_pose.pose.orientation.z;
  cin >> goal_msg.goal.target_pose.pose.orientation.w;
/*
  cin >> t;
  cout << t << endl;
  initPose_msg.header.stamp = ros::Time::now();
  initPose_msg.header.frame_id = "map";
  cin >> initPose_msg.pose.pose.position.x;
  cin >> initPose_msg.pose.pose.position.y;
  initPose_msg.pose.pose.position.z = 0;
  cin >> t;
  cin >> initPose_msg.pose.pose.orientation.x;
  cin >> initPose_msg.pose.pose.orientation.y;
  cin >> initPose_msg.pose.pose.orientation.z;
  cin >> initPose_msg.pose.pose.orientation.w;
*/
}

void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){
//nav_msgs 토픽의 Odometry 메세지를 받음.
  if(!isFirst1){
    initPose_msg.header.stamp = ros::Time::now();
    initPose_msg.header.frame_id = "map";
    initPose_msg.pose.pose.position.x = msg->pose.pose.position.x;
    initPose_msg.pose.pose.position.y = msg->pose.pose.position.y;
    initPose_msg.pose.pose.position.z = 0;
    initPose_msg.pose.pose.orientation.x = msg->pose.pose.orientation.x;
    initPose_msg.pose.pose.orientation.y = msg->pose.pose.orientation.y;
    initPose_msg.pose.pose.orientation.z = msg->pose.pose.orientation.z;
    initPose_msg.pose.pose.orientation.w = msg->pose.pose.orientation.w;
    isFirst1 = true;
    pubInitialPose_ptr->publish(initPose_msg);
  }
}


int main(int argc, char** argv){

  ros::init(argc, argv, "nav_control");

  ros::NodeHandle n;

  n.param<string>("nav_control/txtSave_path", txtSave_path_, "");
  cout<< "경로 : " << txtSave_path_ << endl;
  //Publisher
  ros::Publisher pubInitialPose = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose",10);
  ros::Publisher pubGoal = n.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal",10);
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  pubInitialPose_ptr = &pubInitialPose;

  ros::Rate rate(20.0);
  ros::Time beforeTime;

  int duration_time = 2;


  while(beforeTime == ros::Time(0)){ // 확실한 beforeTime을 받아오기 위해
    beforeTime = ros::Time::now();
  }


  readLogfile();
  bool isFirst2 = false;
  while(n.ok()){
    if(ros::Time::now() - beforeTime > ros::Duration(duration_time)){ // 5초마다 동작중임을 알림
      if(!isFirst2){
        cout << "send goal" << endl;
        pubGoal.publish(goal_msg);
        isFirst2 = true;
      }else{
        cout << "nav_control 동작 중" << endl;
      }
      beforeTime = ros::Time::now();
    }

    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
