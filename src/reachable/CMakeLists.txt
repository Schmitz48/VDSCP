project(VDSProject_test CXX C)
cmake_minimum_required(VERSION 2.6.2)


add_executable(VDSProject_reachable main_test.cpp Reachable.cpp Reachable.h Tests.h ReachableInterface.h)
target_link_libraries(VDSProject_reachable Manager)
target_link_libraries(VDSProject_reachable gtest gtest_main pthread)


