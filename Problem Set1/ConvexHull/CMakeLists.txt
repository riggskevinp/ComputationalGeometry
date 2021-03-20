# SPECIFY THE MINIMUM VERSION OF CMAKE REQUIRED
cmake_minimum_required(VERSION 2.8.12)


# SPECIFY YOUR PROJECT NAME
PROJECT(ConvexHull)


# ENABLE C++11
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)


# MAKE SURE OPENCV IS INSTALLED CORRECTLY
find_package( OpenCV REQUIRED )


# INCLUDE OPENCV DIRECTORIES
include_directories( ${OpenCV_INCLUDE_DIRS})




# MACRO TO COMPILE CPP FILES
# Do Not Edit
MACRO(add_example name)
  ADD_EXECUTABLE(${name} ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS} )
  add_custom_command(
          TARGET ${name} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy
                  ${CMAKE_SOURCE_DIR}/hand.jpg
                  ${CMAKE_CURRENT_BINARY_DIR}/hand.jpg)
ENDMACRO()


# COMPILE CPP FILES USING THIS LINE
######## EDIT THE FILE NAMES ########
add_example(main)
#add_example(sampleCode2) and so on
