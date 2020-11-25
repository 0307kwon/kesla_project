#include <ros/ros.h>
#include <mode_decider/mode_decider.hpp>
#include <kesla_msg/DoneService.h>
#include <string>
#include <sstream>

int mode;

enum MISSION_MODE{
  MODE_NONE,
  MODE_TRAFFIC_SIGN,
  MODE_EXPLORATION,
  MODE_MAP_SAVE,
  MODE_NAVIGATION,
};


bool msgCallback(kesla_msg::DoneService::Request &req,
                kesla_msg::DoneService::Response &res){

  if(req.myRequest.compare("traffic_sign") == 0){
    mode = MODE_TRAFFIC_SIGN;
  }else if(req.myRequest.compare("exploration") == 0){
    mode = MODE_EXPLORATION;
  }else if(req.myRequest.compare("map_save") == 0){
    mode = MODE_MAP_SAVE;
  }else if(req.myRequest.compare("navigation") == 0){
    mode = MODE_NAVIGATION;
    // 종료지점에서 밖으로 탈출
  }else{
    mode = MODE_NONE;
  }
  std::cout << "받은 메세지 :" + req.myRequest << std::endl;
  return true;

}
  /*---------------------------------
  if(msg->data.compare("MODE_TRAFFIC_LIGHT") == 0){
    //msg->data => (*msg).data 라는 의미.
    //msg값과 MODE_TRAFFIC_LIGHT값이 같으면 0을 리턴
    //string형은 비교할 때 주의해야 하므로 #include <string> ->compare()

  ----------------------------------*/
  /*else(){
    std::cout << "알 수 없는 입력입니다." << '\n';
  }*/
  //msg->stamp = ros::Time::now();
  //msg->data = ;

  //pub.publish(msg);



mode_decider::mode_decider(int argc, char** argv){

  int mymode = MODE_NONE; // 0은 모드가 없음
  int before_mymode = mymode; // 이전 모드 저장 //

  ros::init(argc,argv,"mode_decider");//노드명 초기화

  ros::NodeHandle nh;//ROS 시스템과 통신을 위한 노드 핸들 선언

  /*----------------------------------------------------

  ros::Publisher pub = nh.advertise<kesla_msg::KeslaMsg>("/kesla/mode",10);
  //퍼블리셔 선언, kesla_msg패키지의 KeslaMsg파일을 이용한 pub를 작성한다.
  //토픽명은 KselaMsg_gwang이며 queue사이즈를 10으로 설정한것.
  ros::Subscriber sub = nh.subscribe<kesla_msg::KeslaMsg>("/kesla/detection",10, boost::bind(msgCallback,_1,&mymode) );
  //콜백함수 호출을 위한 함수로써, 메세지가 수신되기를 대기, 수신되었을 경우 콜백함수를 실행한다.

  ros::Rate rate(20.0);
  //루프주기, 20hz이므로 0.05초 간격으로 반복된다.

  kesla_msg::KeslaMsg msg;  /// KeslaMsg 메시지 파일형식으로 msg라는 메시지를 선언

  -----------------------------------------------------*/
  //Service
  kesla_msg::DoneService req_finish;    //req 메세지 선언
  kesla_msg::DoneService res_finish;
  ros::ServiceClient clientNavDone = nh.serviceClient<kesla_msg::DoneService>("node_controller/changeMode");
  //boost::bind(msgCallback,_1,&mymode)
  ros::ServiceServer serverNavDone = nh.advertiseService("mode_decider/changeMode", msgCallback);

  /*
  if(!req_finish.myRequest.compare("navigation_finish")){
    res_finish.response.myResponse = "탐색완료";
    cout << res_finish.myResponse << endl;
  }else{
    res_finish.response.myRespone = "탐색 대기중";
  }
  ㄴ이게 필요가 있나?*/

  while (nh.ok()) {
    mymode = mode;
    if(mymode == before_mymode){
    }else if(mymode != before_mymode){
        std::cout << "sending msg : " << mymode << std::endl;    // data 메시지를 표시한다
        std::stringstream temp;
        temp << mymode;                                  //int to string 변환
        req_finish.request.myRequest = temp.str();
        clientNavDone.call(req_finish);                  //메세지를 발행
        before_mymode = mymode;
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

    ros::Rate rate(20.);
    ros::spinOnce(); //subscribe한테 이게 없으면 안됨.
    rate.sleep();
  }



  std::cout << "실행 완료" << std::endl;


}
