#include <kesla_msg/KeslaMsg.h>
#include <node_controller/cpp_roslaunch.hpp>

class node_controller{

private:
  static cpp_roslaunch node_admin; // 노드 관리자 (여러개의 노드를 관리);
  static int mode;

public:
  node_controller(int argc, char** argv);

  static void modeCallback(const kesla_msg::KeslaMsg::ConstPtr& msg);




};
