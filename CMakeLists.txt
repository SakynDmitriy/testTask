cmake_minimum_required(VERSION 3.20)

project(testTask LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})

add_subdirectory(client)
add_subdirectory(protoDir)
add_subdirectory(server)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
message("-- Version: -- "  ${QT_VERSION_MAJOR} " -- ")

target_link_libraries(server proto3 ${Protobuf_LIBRARIES})
target_link_libraries(client proto3 ${Protobuf_LIBRARIES})

include(GNUInstallDirs)

message("VALUE__---- ${CMAKE_INSTALL_PREFIX}  ${CMAKE_CURRENT_LIST_DIR}/../protoDir")
