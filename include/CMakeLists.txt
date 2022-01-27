INCLUDE(FindProtobuf)
find_package(Protobuf REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIRS})

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER packet.proto)

protobuf_generate_python(PROTO_PY packet.proto)

add_library(PACKET_LIB ${PROTO_SRC} ${PROTO_HEADER})
