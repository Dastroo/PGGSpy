cmake_minimum_required(VERSION 3.21)
project(PGGSpy)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -I/usr/include/python3.8")

#   finding libxml2
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp PGGSpy.h Module.h Server.h Audio.cpp Audio.h)
target_link_libraries(${PROJECT_NAME} xml2 SDL2 SDL2_mixer python3.8)