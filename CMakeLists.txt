cmake_minimum_required(VERSION 2.8.3)
project(image_rec)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS roscpp  sensor_msgs image_transport cv_bridge)

## Generate added messages and services with any dependencies listed here
generate_messages(DEPENDENCIES std_msgs)

catkin_package()

# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${Opencv_LIBS}
)

add_executable(image_rec src/image_rec.cpp)
target_link_libraries(image_rec ${catkin_LIBRARIES})
add_dependencies(image_rec image_rec_generate_messages_cpp)


