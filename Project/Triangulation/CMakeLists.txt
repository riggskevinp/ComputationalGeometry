cmake_minimum_required(VERSION 3.5)

project(Triangulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# MAKE SURE OPENCV IS INSTALLED CORRECTLY
find_package( OpenCV REQUIRED )


# INCLUDE OPENCV DIRECTORIES
include_directories( ${OpenCV_INCLUDE_DIRS})


################################
# GTest
################################
ADD_SUBDIRECTORY (googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${OpenCV_LIBS})


################################
# Unit Tests
################################
# Add test cpp file
add_executable( runUnitTests
    testgtest.cpp
    quadedge.cpp
    quadedge.h
    face.h
    face.cpp
    vertex.h
    vertex.cpp
    delaunay.h
    delaunay.cpp
    csv.h

    )

add_custom_command(
          TARGET runUnitTests POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy
                  ${CMAKE_SOURCE_DIR}/test/test.csv
                  ${CMAKE_CURRENT_BINARY_DIR}/test.csv
                  )

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main ${OpenCV_LIBS})
add_test( runUnitTests runUnitTests )



add_executable(Triangulation
    main.cpp
    quadedge.cpp
    quadedge.h
    face.h
    face.cpp
    vertex.h
    vertex.cpp
    delaunay.h
    delaunay.cpp
    csv.h
    )
target_link_libraries(Triangulation ${OpenCV_LIBS})
