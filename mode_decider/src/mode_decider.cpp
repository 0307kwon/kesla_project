#include <ros/ros.h>
#include <mode_decider/mode_decider.hpp>
#include <kesla_msg/KeslaMsg.h>
#include <string>
#include <sstream>

enum MISSION_MODE{
  MODE_NONE,
  MODE_TRAFFIC_SIGN,
  MODE_EXPLORATION,
};


void msgCallback(const kesla_msg::KeslaMsg::ConstPtr& msg, int* mode){


  if(msg->data.compare("traffic_sign") == 0){
    (*mode) = MODE_TRAFFIC_SIGN;
  }
  else if(msg->data.compare("exploration") == 0){
    (*mode) = MODE_EXPLORATION;
  }
  else{
    (*mode) = MODE_NONE;
  }
  std::cout << "메세지 :" + msg->data << std::endl;
  /*---------------------------------
  if(msg->data.compare("MODE_TRAFFIC_LIGHT") == 0){
    //msg->data => (*msg).data 라는 의미.
    //msg값과 MODE_TRAFFIC_LIGHT값이 같으면 0을 리턴
    //string형은 비교할 때 주의해야 하므로 #include <string> ->compare()
    (*mode) = "1";
  }
  if(msg->data.compare("MODE_INTERSECTION") == 0){
    (*mode) = "2";
  }
  if(msg->data.compare("MODE_PARKING") == 0){
    (*mode) = "3";
  }
  if(msg->data.compare("MODE_TUNNEL") == 0){
    (*mode) = "4";
  }
  ----------------------------------*/
  /*else(){
    std::cout << "알 수 없는 입력입니다." << '\n';
  }*/
  //msg->stamp = ros::Time::now();
  //msg->data = ;

  //pub.publish(msg);


}



mode_decider::mode_decider(int argc, char** argv){

  int mymode = MODE_NONE; // 0은 모드가 없음
  int before_mymode = mymode; // 이전 모드 저장 //
  ros::Time last_Time;
  ros::init(argc,argv,"mode_decider");//노드명 초기화

  ros::NodeHandle nh;//ROS 시스템과 통신을 위한 노드 핸들 선언

  ros::Publisher pub = nh.advertise<kesla_msg::KeslaMsg>("/kesla/mode",10);
  //퍼블리셔 선언, kesla_msg패키지의 KeslaMsg파일을 이용한 pub를 작성한다.
  //토픽명은 KselaMsg_gwang이며 queue사이즈를 10으로 설정한것.
  ros::Subscriber sub = nh.subscribe<kesla_msg::KeslaMsg>("/kesla/detection",10, boost::bind(msgCallback,_1,&mymode) );
  //콜백함수 호출을 위한 함수로써, 메세지가 수신되기를 대기, 수신되었을 경우 콜백함수를 실행한다.

  ros::Rate rate(20.0);
  //루프주기, 20hz이므로 0.05초 간격으로 반복된다.

  kesla_msg::KeslaMsg msg;  /// KeslaMsg 메시지 파일형식으로 msg라는 메시지를 선언



  while (nh.ok()) {
    ros::Rate rate2(5.0);
    ros::Time Present_Time;
    if(mymode == before_mymode){
      last_Time = ros::Time::now();
        std::cout << "메세지 입력 대기중..." << '\n';
    }else if(mymode != before_mymode){
    Present_Time = ros::Time::now();
    std::cout << Present_Time - last_Time << std::endl;
      if(Present_Time - last_Time < ros::Duration(3.0)){
        std::cout << "msg : " << mymode << std::endl;    // data 메시지를 표시한다
        std::stringstream temp;
        temp << mymode;                //int to string 변환
        msg.data = temp.str();
        pub.publish(msg);                 //메세지를 발행
      }
      else{
        before_mymode = mymode;
      }
    }

    /*-------------------------------------
    msg.stamp = ros::Time::now();
    msg.data = mymode;
    if(mymode.compare("MODE_NONE")){
      std::cout << "메세지 입력 대기중.." << '\n';
    }

    else{
      std::cout << "msg : " << msg.data << std::endl;     // data 메시지를 표시한다

      pub.publish(msg);                   //메세지를 발행
    }
    ------------------------------------------*/
    /* code */



    ros::spinOnce(); //subscribe한테 이게 없으면 안됨.
    rate.sleep();
  }





  std::cout << "실행 완료" << std::endl;



}
