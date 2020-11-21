#include <ros/ros.h>
#include <string>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <move_base_msgs/MoveBaseActionGoal.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

geometry_msgs::PoseWithCovarianceStamped msg;
/*
msg.header.stamp = ros::Time::now();
msg.header.frame_id = "map";
msg.pose.pose.position.x = -0.550445973873;
msg.pose.pose.position.y = 0.530910432339;
msg.pose.pose.position.z = 0;
msg.pose.pose.orientation.x = 0;
msg.pose.pose.orientation.y = 0;
msg.pose.pose.orientation.z = 0;
msg.pose.pose.orientation.w = 1;
*/
move_base_msgs::MoveBaseActionGoal msg2;
/*
msg2.header.stamp = ros::Time::now();
msg2.goal.target_pose.header.stamp = ros::Time::now();
msg2.goal.target_pose.header.frame_id = "map";
msg2.goal.target_pose.pose.position.x = -0.246008440852;
msg2.goal.target_pose.pose.position.y = -0.462787866592;
msg2.goal.target_pose.pose.position.z = 0;
msg2.goal.target_pose.pose.orientation.x = 0;
msg2.goal.target_pose.pose.orientation.y = 0;
msg2.goal.target_pose.pose.orientation.z = 0;
msg2.goal.target_pose.pose.orientation.w = 1;
*/
void readLogfile(){
  if(freopen("navigation_result/log.txt","r",stdin) == NULL){
		ROS_WARN_STREAM("cannot find file location parameter");
		return;
	}

	string t;
	getline(cin,t);
  cout << t << endl;
}


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
  ros::Publisher pubInitialPose = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose",10);
  ros::Publisher pubGoal = n.advertise<move_base_msgs::MoveBaseActionGoal>("/move_base/goal",10);




  ros::Rate rate(20.0);
  ros::Time beforeTime;

  int duration_time = 1;
  bool isFirst = true;

  while(beforeTime == ros::Time(0)){ // 확실한 beforeTime을 받아오기 위해
    beforeTime = ros::Time::now();
  }


  readLogfile();



  while(n.ok()){
    if(ros::Time::now() - beforeTime > ros::Duration(duration_time)){ // 5초마다 동작중임을 알림
      if(isFirst){
        cout << "send initial pose" << endl;
        beforeTime = ros::Time::now();

        pubInitialPose.publish(msg);


        pubGoal.publish(msg2);
      }else{
        cout << "nav_control 동작 중" << endl;
      }
    }

    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
