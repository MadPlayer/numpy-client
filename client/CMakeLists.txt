

set(client_sources
  client.cpp
  message.cpp
  tensor.cpp
  )
list(TRANSFORM client_sources PREPEND "src/")

find_package(Protobuf REQUIRED)

add_executable(client
  ${client_sources}
  )

link_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

target_sources(client PUBLIC
  ${client_sources}
  )

target_link_libraries(client
  ${Protobuf_LIBRARY}
  PACKET_LIB
  pthread)

target_include_directories(client PUBLIC
  ${ASIO_HEADER}
  ${CMAKE_CURRENT_BINARY_DIR}/../include/
  include
  ${PROTO_HEADER}
  ${Protobuf_INCLUDE_DIRS}
  )
