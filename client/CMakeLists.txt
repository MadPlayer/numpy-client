set(client_sources
  client.cpp
  message.cpp
  tensor.cpp
  session.cpp
  )
list(TRANSFORM client_sources PREPEND "src/")

find_package(Protobuf REQUIRED)

add_executable(client)

target_sources(client PRIVATE
  ${client_sources}
  )

# XXX:  explicitely require ONNXRUNTIME_DIR
target_link_directories(client PRIVATE
  ${ONNXRUNTIME_DIR}/lib/
  )

target_link_libraries(client
  ${Protobuf_LIBRARY}
  PACKET_LIB
  pthread
  onnxruntime
  )

target_include_directories(client PRIVATE
  include
  ${ONNXRUNTIME_DIR}/include/
  )
