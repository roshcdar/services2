cmake_minimum_required(VERSION 3.22)
project(services)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(services2 main.cpp ListServices.cpp ServiceHandle.cpp ServiceManagerFactory.cpp ServiceManagerObject.cpp)
