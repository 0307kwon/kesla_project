#include <signal.h>
#include <sys/wait.h>
#include <vector>
#include <string>



typedef struct node{
  std::string name;
  pid_t pid;

}Node;

class cpp_roslaunch{

private:
  static struct sigaction act;
  static std::vector<Node> nodes;

public:

  cpp_roslaunch();
  int roslaunch(const char* node_name,const char* launch_name);
  int roslaunch(const char* node_name,const char* launch_name,const char* option);
  int kill_node(const char* node_name);
  void mychild(int sig);
  void init();

};
