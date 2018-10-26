cmake_minimum_required(VERSION 3.12)
project(StateMachine)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp Vertex.hpp Vertex.h Block.hpp Block.h StateMachine.hpp StateMachine.h)