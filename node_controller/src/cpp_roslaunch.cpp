# include <node_controller/cpp_roslaunch.hpp>
#include <signal.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

std::vector<Node> cpp_roslaunch::nodes;
struct sigaction cpp_roslaunch::act;

void cpp_roslaunch::mychild(int sig){ // 좀비 child 제거 함수 (시그널 함수)
  int status;
  pid_t pid;
  while( ( pid = waitpid(-1,&status,WNOHANG) ) > 0 ){
    for(int i=0; i<nodes.size(); i++){
      if(nodes[i].pid == pid){
        nodes.erase(nodes.begin()+i);
        std::cout << "node 관리 배열에서 삭제하였습니다. 현재 노드갯수 :  " << cpp_roslaunch::nodes.size() <<std::endl;
        break;
      }
    }
  }
//
}

cpp_roslaunch::cpp_roslaunch(){
  std::cout << "cpp_roslaunch is being executed" << std::endl;

  act.sa_handler = (void (*)(int))&cpp_roslaunch::mychild;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGCHLD,&act,0);
}

int cpp_roslaunch::roslaunch(const char* node_name,const char* launch_name){ // 0이면 실패 1이면 성공 (launch 파일의 실행 갯수는 무조건 1개여야함)

  Node node;
  node.name = node_name;
  node.pid = fork();

  nodes.push_back(node);

  if(node.pid == -1) { //-1 이면 fork생성 에러
		std::cout << "can't fork error" << std::endl;
		return 0;
	}

	if(node.pid == 0) { //0이면 자식 프로세스
    execlp("roslaunch","roslaunch",node_name,launch_name,NULL);
  }else{ // 부모 프로세스
    std::cout << node.name << " 실행 완료" << std::endl;
    return 1;
  }
}

int cpp_roslaunch::roslaunch(const char* node_name,const char* launch_name,const char* option){ //roslaunch의 추가적인 인자 버전//
  Node node;
  node.name = node_name;
  node.pid = fork();

  nodes.push_back(node);

  if(node.pid == -1) { //-1 이면 fork생성 에러
		std::cout << "can't fork error" << std::endl;
		return 0;
	}


	if(node.pid == 0) { //0이면 자식 프로세스
    execlp("roslaunch","roslaunch",node_name,launch_name,option,NULL);
  }else{ // 부모 프로세스
    std::cout << node.name << " 실행 완료" << std::endl;
    return 1;
  }
}

void cpp_roslaunch::init(){ // 모든 노드를 종료하고 초기 상태로 되돌립니다.
  for(int i=0; i<nodes.size(); i++){
    kill(nodes[i].pid,SIGTERM);
  }
  std::cout << "모든 노드를 강제종료하였습니다." << std::endl;
}

int cpp_roslaunch::kill_node(const char* node_name){
  for(int i=0; i<nodes.size(); i++){
    if(nodes[i].name.compare(node_name) == 0){
      kill(nodes[i].pid,SIGTERM);
      std::cout << node_name <<" 노드를 강제종료하였습니다." << std::endl;
      return 1;
    }
  }
  std::cout << "강제종료할 노드를 찾지 못했습니다.." << std::endl;
  return 0;
}
