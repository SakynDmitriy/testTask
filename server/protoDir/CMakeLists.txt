cmake_minimum_required(VERSION 3.20)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER transMessg.proto)
add_library(proto3 ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(proto3)
