#include <ros/ros.h>
#include <mode_decider/mode_decider.hpp>



int main(int argc, char** argv){


  mode_decider node(argc,argv);


  std::cout << "실행 완료" << std::endl;


  return 0;
}
