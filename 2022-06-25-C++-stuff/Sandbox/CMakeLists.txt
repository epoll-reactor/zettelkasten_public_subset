cmake_minimum_required(VERSION 3.20)
project(Sandbox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-O0 -fno-inline")
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -ftree-vectorize -ffast-math -march=native -flto")

#add_compile_options(
#  -fsanitize=address -fno-omit-frame-pointer
#  -fsanitize=undefined -fno-sanitize-recover=all
#  -fsanitize-address-use-after-scope
#)
#link_libraries(
#  -fsanitize=address -fno-omit-frame-pointer
#  -fsanitize=undefined -fno-sanitize-recover=all
#  -fsanitize-address-use-after-scope
#)
#if(CMAKE_CXX_COMPILER MATCHES "clang")
#  add_compile_options(
#    -fsanitize=cfi -fvisibility=default -flto
#  )
#  link_libraries(
#    -fsanitize=cfi -fvisibility=default -flto
#  )
#endif()

#include_directories(/usr/include/python2.7)

#SET("OpenCV_DIR" "/usr/share/OpenCV")
#find_package(OpenCV REQUIRED)
#MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
#MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_executable(Sandbox main.cpp)
target_link_libraries(Sandbox PRIVATE ${OpenCV_LIBS})