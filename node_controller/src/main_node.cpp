#include <ros/ros.h>
#include <node_controller/node_controller.hpp>
#include <iostream>



int main(int argc, char** argv){
  std::cout << "실행 완료2" << std::endl;

  node_controller node(argc,argv);

  return 0;
}
