cmake_minimum_required(VERSION 3.5)

project(winapi LANGUAGES CXX)

add_definitions(-DCURL_STATICLIB)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "wineg++")

add_executable(winapi main.cpp)

target_link_libraries(winapi iphlpapi wldap32 simdjson curl)
