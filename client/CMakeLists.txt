set(client_sources client.cpp)
list(TRANSFORM client_sources PREPEND "src/")

add_executable(client
  "${client_sources}"
  ${PROTO_PACKET}
  )

target_link_libraries(client
  -lpthread)

target_include_directories(client PUBLIC
  ${ASIO_HEADER}
  ${CMAKE_CURRENT_BINARY_DIR}/../include/
  ${PROTO_HEADER}
  )

message(
  ${CMAKE_CURRENT_BINARY_DIR}/../include/
  )
