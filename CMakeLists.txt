cmake_minimum_required(VERSION 2.8.3)
project(vasco_npm)

set (CMAKE_VERBOSE_MAKEFILE ON)

##################################################
# OS detection (needed for GL support)

if (WIN32)
  add_definitions (-DWIN32)
else (WIN32)
  if (APPLE)
    add_definitions (-DOSX)
  endif (APPLE)
  if (CMAKE_SYSTEM_NAME MATCHES Linux)
    add_definitions (-DLINUX)
  endif (CMAKE_SYSTEM_NAME MATCHES Linux)
  if (CMAKE_SYSTEM_NAME MATCHES OpenBSD)
    add_definitions (-DOPENBSD)
  endif (CMAKE_SYSTEM_NAME MATCHES OpenBSD)
endif(WIN32)

##################################################
# ROS stuff

find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  sfl2
)

add_message_files(
  DIRECTORY msg
  FILES
  Scan.msg
)

generate_messages()

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  CATKIN_DEPENDS roscpp sfl2
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_library (vasco-plugin MODULE src/vasco-plugin.cpp)
target_link_libraries (vasco-plugin npm ${catkin_LIBRARIES})
#set_target_properties (vasco-plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${NPM_PLUGIN_PATH})
