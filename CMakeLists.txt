project(SkipList)
set(CMAKE_CXX_FLAGS "-std=c++1y -g -Wall -O3")
cmake_minimum_required(VERSION 2.8)
file(GLOB SRC_LIST
	"*.hpp"
	"*.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_LIST})

