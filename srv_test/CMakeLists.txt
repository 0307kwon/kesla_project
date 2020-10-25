cmake_minimum_required(VERSION 2.8.3)
project(srv_test)

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
  LIBRARIES srv_test
  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include
)


add_executable(srv_test src/main.cpp)
add_dependencies(srv_test ${${PROJECT_NAME}_EXPORTED_TARGETS}
${catkin_EXPORTED_TARGETS})
target_link_libraries(srv_test ${catkin_LIBRARIES})
