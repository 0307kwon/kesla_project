cmake_minimum_required(VERSION 2.8.3)
project(nav_control)
add_compile_options(-std=c++11)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  kesla_msg
  message_generation
)

#add_message_files(FILES KeslaMsg.msg)


generate_messages(DEPENDENCIES std_msgs)


catkin_package(
  INCLUDE_DIRS include
  LIBRARIES nav_control
  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include
)


add_executable(nav_control src/main_node.cpp)
add_dependencies(nav_control ${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS})
target_link_libraries(nav_control ${catkin_LIBRARIES})
