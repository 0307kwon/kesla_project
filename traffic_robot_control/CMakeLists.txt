cmake_minimum_required(VERSION 2.8.3)
project(traffic_robot_control)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
  kesla_msg
)

#add_message_files(FILES KeslaMsg.msg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES traffic_robot_control
  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
 #include
  ${catkin_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include
)


add_executable(traffic_robot_control 
	src/main_node.cpp
)

target_link_libraries(traffic_robot_control ${catkin_LIBRARIES})

add_dependencies(traffic_robot_control ${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS})




