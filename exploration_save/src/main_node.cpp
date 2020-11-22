#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <kesla_msg/DoneService.h>
#include <string>
#include <geometry_msgs/PointStamped.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

using namespace std;


//전역 변수
ros::Publisher* pubClickedPoint_ptr;
ros::ServiceClient* clientExplor_ptr;
bool isFirst = false;
stringstream save_ss;
string txtSave_path_;
nav_msgs::Odometry turtle_pose;
//ros::ServiceClient clientMode = n.serviceClient<kesla_msg::DoneService>("explore_server/sendNav");
//


bool srv_callback(kesla_msg::DoneService::Request &req,
                  kesla_msg::DoneService::Response &res);
void sendClickedPoint(float x, float y);
void msgCallback(const nav_msgs::Odometry::ConstPtr& msg);


bool srv_callback(kesla_msg::DoneService::Request &req,
                  kesla_msg::DoneService::Response &res){
  cout << req.myRequest <<endl;
  if(!req.myRequest.compare("finished")){
    ROS_ERROR("exploration finished");
    //clientMode.call(req);
    //텍스트 파일 저장//
    save_ss << "end_position "<< turtle_pose.pose.pose.position.x << " " << turtle_pose.pose.pose.position.y << std::endl;
    save_ss << "end_quaternion " << turtle_pose.pose.pose.orientation.x << " " << turtle_pose.pose.pose.orientation.y << " "
                              << turtle_pose.pose.pose.orientation.z << " " << turtle_pose.pose.pose.orientation.w << std::endl;
    ofstream outFile(txtSave_path_.c_str());
    cout<< "이름" << txtSave_path_ << endl;
    if(outFile.is_open()){
      outFile << save_ss.str();
      outFile.close();
    }
    //다음 모드로 이동//
    kesla_msg::DoneService req_explor;
    req_explor.request.myRequest = "map_save";
    clientExplor_ptr->call(req_explor);

  }else if(!req.myRequest.compare("excuted")){
    res.myResponse = "excuted success";
    float scale = 0.9;

    // 실제 실험용
    /*
    sendClickedPoint(-0.1,scale-0.45);
    sendClickedPoint(scale*2,scale-0.45);
    sendClickedPoint(scale*2,-scale-0.45);
    sendClickedPoint(-0.1,-scale-0.45);
    sendClickedPoint(-0.1,scale-0.45);
    sendClickedPoint(0,0);
    */

    //가제보용
    sendClickedPoint(-scale*2,scale*2);
    sendClickedPoint(scale*2,scale*2);
    sendClickedPoint(scale*2,-scale);
    sendClickedPoint(-scale*2,-scale);
    sendClickedPoint(-scale*2,scale*2);
    sendClickedPoint(0,0);
  }else{
    res.myResponse = "fail";
  }
  return true;
}

void sendClickedPoint(float x, float y){
  geometry_msgs::PointStamped msg;
  msg.header.frame_id = "map";
  msg.point.x = x; msg.point.y = y; msg.point.z = 0;
  pubClickedPoint_ptr->publish(msg);
  ROS_ERROR("kesla - sendClickedPoint");
}


void msgCallback(const nav_msgs::Odometry::ConstPtr& msg){
//nav_msgs 토픽의 Odometry 메세지를 받음.
  turtle_pose.pose.pose.position.x = msg->pose.pose.position.x;
  turtle_pose.pose.pose.position.y = msg->pose.pose.position.y;
  turtle_pose.pose.pose.position.z = msg->pose.pose.position.z;
  turtle_pose.pose.pose.orientation.x = msg->pose.pose.orientation.x;
  turtle_pose.pose.pose.orientation.y = msg->pose.pose.orientation.y;
  turtle_pose.pose.pose.orientation.z = msg->pose.pose.orientation.z;
  turtle_pose.pose.pose.orientation.w = msg->pose.pose.orientation.w;

  if(!isFirst){
    save_ss << "start_position "<< turtle_pose.pose.pose.position.x << " " << turtle_pose.pose.pose.position.y << std::endl;
    save_ss << "start_quaternion " << turtle_pose.pose.pose.orientation.x << " " << turtle_pose.pose.pose.orientation.y << " "
                              << turtle_pose.pose.pose.orientation.z << " " << turtle_pose.pose.pose.orientation.w << std::endl;
    isFirst = true;
  }
}


int main(int argc, char** argv){

  ros::init(argc, argv, "exploration_save");
  ros::NodeHandle n;

  n.param<string>("exploration_save/txtSave_path", txtSave_path_, "");
  //service
  ros::ServiceServer clientDone = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  ros::ServiceServer clientExcuted = n.advertiseService("explore_server/sendExplorExcuted",srv_callback);
  ros::ServiceClient clientExplor = n.serviceClient<kesla_msg::DoneService>("mode_decider/changeMode");
  clientExplor_ptr = &clientExplor;
  //subscriber
  ros::Subscriber sub = n.subscribe<nav_msgs::Odometry>("/odom",10,msgCallback);
  //Publisher
  ros::Publisher pubClickedPoint = n.advertise<geometry_msgs::PointStamped>("/clicked_point",10);
  pubClickedPoint_ptr = &pubClickedPoint;

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
