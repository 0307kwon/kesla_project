cmake_minimum_required(VERSION 2.8.3)
project(mode_decider)

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
  LIBRARIES mode_decider
  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include
)


add_executable(mode_decider src/main_node.cpp src/mode_decider.cpp)
add_dependencies(mode_decider ${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS})
target_link_libraries(mode_decider ${catkin_LIBRARIES})
