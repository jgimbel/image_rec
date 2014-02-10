cmake_minimum_required(VERSION 2.8.3)
project(image_rec)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS roscpp  sensor_msgs image_transport cv_bridge)

## Generate added messages and services with any dependencies listed here
#generate_messages(DEPENDENCIES std_msgs)

catkin_package()

# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${Opencv_LIBS}
)


## Declare a cpp library
# add_library(image_rec
#   src/${PROJECT_NAME}/image_rec.cpp
# )

## Declare a cpp executable
 add_executable(image_rec_node src/image_rec.cpp)
# add_executable(image_publish_node src/image_publish.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(image_rec_node image_rec_generate_messages_cpp)

## Specify libraries to link a library or executable target against
 target_link_libraries(image_rec_node ${catkin_LIBRARIES})
# target_link_libraries(image_publish_node ${catkin_LIBRARIES})

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS image_rec image_rec_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############



