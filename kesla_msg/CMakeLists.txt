cmake_minimum_required(VERSION 2.8.3)
project(kesla_msg)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  message_generation
)

add_message_files(FILES KeslaMsg.msg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
# INCLUDE_DIRS include
  CATKIN_DEPENDS std_msgs
#  DEPENDS system_lib
)


