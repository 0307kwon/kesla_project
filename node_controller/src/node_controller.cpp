#include <ros/ros.h>
#include <node_controller/node_controller.hpp>
#include <cstring>

//static 변수 초기화//
cpp_roslaunch node_controller::node_admin;
int node_controller::mode = 0;

enum MISSION_MODE{
  MODE_NONE,
  MODE_TRAFFIC_SIGN,
  MODE_EXPLORATION,
};


void node_controller::modeCallback(const kesla_msg::KeslaMsg::ConstPtr& msg){
  std::cout << "mode :" + msg->data << std::endl;
  const char* received_mode = msg->data.c_str();

  if(mode != atoi(received_mode)){
    mode = atoi(received_mode);
    if(mode == MODE_TRAFFIC_SIGN){
      node_admin.init();
      node_admin.roslaunch("traffic_robot_control","control.launch");
      node_admin.roslaunch("opencv","opencv.launch");
    }else if(mode == MODE_EXPLORATION){
      node_admin.init();
      node_admin.roslaunch("turtlebot3_slam","turtlebot3_slam.launch","slam_methods:=frontier_exploration");
      node_admin.roslaunch("exploration_save","exploration_save.launch");
      //node_admin.roslaunch("camera_to_world","camera.launch");
    }
  }
}

node_controller::node_controller(int argc, char** argv){

  ros::init(argc,argv,"node_controller");

  ros::NodeHandle nh;

  ros::Publisher pub = nh.advertise<kesla_msg::KeslaMsg>("KeslaMsg_kwon",10);
  ros::Subscriber sub = nh.subscribe("/kesla/mode", 10, node_controller::modeCallback);

  ros::Rate rate(20.0);

  kesla_msg::KeslaMsg msg;

  while(nh.ok()){

    msg.stamp = ros::Time::now();
    msg.data = "so tired";

    pub.publish(msg);

    std::cout << "node_controller 동작중" << std::endl;

    ros::spinOnce();
    rate.sleep();
  }

  std::cout << "실행 완료" << std::endl;

}
