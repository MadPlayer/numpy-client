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

target_link_libraries(client
  ${Protobuf_LIBRARY}
  PACKET_LIB
  pthread
  )

target_include_directories(client PRIVATE
  include
  )
