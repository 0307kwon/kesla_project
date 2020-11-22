#include <ros/ros.h>
#include <node_controller/node_controller.hpp>
#include <cstring>
#include <kesla_msg/DoneService.h>

using namespace std;

//static 변수 초기화//
cpp_roslaunch node_controller::node_admin;
int node_controller::mode = 0;

enum MISSION_MODE{
  MODE_NONE,
  MODE_TRAFFIC_SIGN,
  MODE_EXPLORATION,
  MODE_MAP_SAVE,
  MODE_NAVIGATION,
};

bool node_controller::modeCallback(kesla_msg::DoneService::Request &req,kesla_msg::DoneService::Response &res){
  std::cout << "mode :" << req.myRequest << std::endl;
  const char* received_mode = req.myRequest.c_str();

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
    }else if(mode == MODE_MAP_SAVE){
      node_admin.roslaunch("map_server", "map_saver.launch");
    }else if(mode == MODE_NAVIGATION){
      node_admin.init();
      node_admin.roslaunch("nav_control","nav_control.launch");
      node_admin.roslaunch("turtlebot3_navigation","turtlebot3_navigation.launch","map_file:=$HOME/navigation_result/savemap.yaml");
    }
  }
}


node_controller::node_controller(int argc, char** argv){

  ros::init(argc,argv,"node_controller");

  ros::NodeHandle nh;
  ros::ServiceServer serverNavDone = nh.advertiseService("node_controller/changeMode", modeCallback);
/*-------------------------------------------------------------------------------------
  ros::Publisher pub = nh.advertise<kesla_msg::KeslaMsg>("KeslaMsg_kwon",10);
  ros::Subscriber sub = nh.subscribe("/kesla/mode", 10, node_controller::modeCallback);
--------------------------------------------------------------------------------------*/
  ros::Rate rate(20.0);

  //kesla_msg::KeslaMsg msg;
  ros::Time beforeTime = ros::Time::now();

  while(beforeTime == ros::Time(0)){ // 확실한 beforeTime을 받아오기 위해
    beforeTime = ros::Time::now();
  }

  while(nh.ok()){
    /*
    msg.stamp = ros::Time::now();
    msg.data = "so tired";
    pub.publish(msg);
    */
    if(ros::Time::now() - beforeTime > ros::Duration(3) && beforeTime != ros::Time(0)) {
      beforeTime = ros::Time::now();
      std::cout << "node_controller 동작중" << std::endl;
    }

    ros::spinOnce();
    rate.sleep();
  }

  std::cout << "실행 완료" << std::endl;

}
