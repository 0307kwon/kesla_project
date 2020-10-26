#include <ros/ros.h>
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

int main(int argc, char** argv){

  ros::init(argc,argv,"exploration_save");
  ros::NodeHandle n;
  ros::ServiceServer client = n.advertiseService("explore_server/sendExplorDone",srv_callback);
  ROS_INFO("Be ready");
  ros::spin();
  return 0;
}
