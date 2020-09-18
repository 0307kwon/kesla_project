#include <ros/ros.h>
#include <cstdlib>
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include <vector>
#include <cmath>

using namespace std;

#define N_TEMP 180

#define SPD 0.02
#define Ang_SPD 0.06




typedef struct gradient{
  double angular;
  double x;
  bool is_vaild;
}Gradient;


vector<string> split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str);
    string temp;

    while (getline(ss, temp, delimiter)) {
        internal.push_back(temp);
    }
    return internal;
}

void modeCallback(const std_msgs::String::ConstPtr& msg, Gradient* left, Gradient* right){
  const char* temp = msg->data.c_str();
  vector<string> gradient = split(msg->data, ',');

  //std::cout << "traffic_robot_control 동작중 : " << gradient[0] << "| " << gradient[1] << std::endl;
  if(gradient[0].compare("empty") == 0){
    left->angular = 0;
    left->is_vaild = false;
  }else{
    left->angular = atof(gradient[0].c_str());
    left->x = atof(gradient[1].c_str());
    left->is_vaild = true;
  }

  if(gradient[2].compare("empty") == 0){
    right->angular = 0;

    right->is_vaild = false;
  }else{
    right->angular = atof(gradient[2].c_str());
    right->x = atof(gradient[3].c_str());
    right->is_vaild = true;
  }

}

int main(int argc, char** argv){

  Gradient left;
  left.angular = 0;
  left.x = 0;
  left.is_vaild = true;

  Gradient right;
  right.angular = 0;
  right.x = 0;
  right.is_vaild = true;

  ros::init(argc,argv,"traffic_robot_control");

  ros::NodeHandle nh;

  ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",10);
  ros::Subscriber sub = nh.subscribe<std_msgs::String>("/kesla/gradient", 10, boost::bind(modeCallback,_1,&left, &right) );

  ros::Rate rate(40.0);

  geometry_msgs::Twist msg;

  int n_temp = 0; // 일정 범위 동안 강제 회전시키기 위한 변수입니다.//

  while(nh.ok()){

    /*
    if(left.is_vaild && right.is_vaild){

      //두쪽다 보일때//

      msg.linear.x = SPD+0.08;

      msg.linear.y = 0;

      msg.linear.z = 0;

      msg.angular.x = 0;

      msg.angular.y = 0;

      msg.angular.z = 0;





    }else if(left.is_vaild == false && right.is_vaild == true){

      //오른쪽만 보일때 - 좌회전 //

      if(right.angular <-35){

        msg.linear.x = 0.05;

        msg.linear.y = 0;

        msg.linear.z = 0;

        msg.angular.x = 0;

        msg.angular.y = 0;

        msg.angular.z = (320-right.x)*0.1/160.0;

      }else{

        //n_temp = 0;

        msg.linear.x = 0.05;

        msg.linear.y = 0;

        msg.linear.z = 0;

        msg.angular.x = 0;

        msg.angular.y = 0;

        msg.angular.z = -0.03*abs(right.angular+25)+0.3;//0.05*right.angular+1.75;

      }



    }else if(left.is_vaild == true && right.is_vaild == false){



      if(left.angular > 35){

        msg.linear.x = 0.05;

        msg.linear.y = 0;

        msg.linear.z = 0;

        msg.angular.x = 0;

        msg.angular.y = 0;

        msg.angular.z = -(left.x)*0.1/160.0;

      }else{

        //n_temp = 0;

        msg.linear.x = 0.05;

        msg.linear.y = 0;

        msg.linear.z = 0;

        msg.angular.x = 0;

        msg.angular.y = 0;

        msg.angular.z = 0.03*abs(left.angular-25)-0.3;

      }

    }

*/

if(left.is_vaild && right.is_vaild){
  //두쪽다 보일때//
  msg.linear.x = 0.05;
  msg.linear.y = 0;
  msg.linear.z = 0;
  msg.angular.x = 0;
  msg.angular.y = 0;
  msg.angular.z = 0;


}else if(left.is_vaild == false && right.is_vaild == true){
  //오른쪽만 보일때 - 좌회전 //
  if(right.angular <-35){
    msg.linear.x = 0.05;
    msg.linear.y = 0;
    msg.linear.z = 0;
    msg.angular.x = 0;
    msg.angular.y = 0;
    msg.angular.z = (320-right.x)*0.1/160.0;
  }else{
    //n_temp = 0;
    msg.linear.x = 0.05;
    msg.linear.y = 0;
    msg.linear.z = 0;
    msg.angular.x = 0;
    msg.angular.y = 0;
    msg.angular.z = -1.0/right.angular+10.0/abs(160-right.x);
  }

}else if(left.is_vaild == true && right.is_vaild == false){

  if(left.angular > 35){
    msg.linear.x = 0.05;
    msg.linear.y = 0;
    msg.linear.z = 0;
    msg.angular.x = 0;
    msg.angular.y = 0;
    msg.angular.z = -(left.x)*0.1/160.0;
  }else{
    //n_temp = 0;
    msg.linear.x = 0.05;
    msg.linear.y = 0;
    msg.linear.z = 0;
    msg.angular.x = 0;
    msg.angular.y = 0;
    msg.angular.z = -1.0/left.angular-10.0/abs(160-left.x);
  }
}

    /*
    if(left.is_vaild && right.is_vaild){
      //두쪽다 보일때//
      msg.linear.x = SPD+0.08;
      msg.linear.y = 0;
      msg.linear.z = 0;
      msg.angular.x = 0;
      msg.angular.y = 0;
      msg.angular.z = 0;


    }else if(left.is_vaild == false && right.is_vaild == true){
      //오른쪽만 보일때 - 좌회전 //
      if(right.angular <-35){
        msg.linear.x = 0.05;
        msg.linear.y = 0;
        msg.linear.z = 0;
        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = (320-right.x)*0.1/160.0;
      }else{
        //n_temp = 0;
        msg.linear.x = 0.05;
        msg.linear.y = 0;
        msg.linear.z = 0;
        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = -0.03*abs(right.angular+25)+0.3;//0.05*right.angular+1.75;
      }

    }else if(left.is_vaild == true && right.is_vaild == false){

      if(left.angular > 35){
        msg.linear.x = 0.05;
        msg.linear.y = 0;
        msg.linear.z = 0;
        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = -(left.x)*0.1/160.0;
      }else{
        //n_temp = 0;
        msg.linear.x = 0.05;
        msg.linear.y = 0;
        msg.linear.z = 0;
        msg.angular.x = 0;
        msg.angular.y = 0;
        msg.angular.z = 0.03*abs(right.angular+25)-0.3;
      }
    }
    */


    pub.publish(msg);

    std::cout << "traffic_robot_control 동작중 : " << left.is_vaild<< "| " << right.is_vaild << "| " << right.angular << std::endl;
    cout << "속도 : " << msg.linear.x << " 각속도 : " << msg.angular.z << endl;
    cout << "left_x : " << left.x << " right_x : " << right.x << endl;

    ros::spinOnce();
    rate.sleep();
  }
}
