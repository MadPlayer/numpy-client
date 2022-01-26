cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(client_sources client.cpp)
list(TRANSFORM client_sources PREPEND "src/")

add_executable(client
  "${client_sources}"
  )

target_link_libraries(client
  -lpthread)

target_include_directories(client PUBLIC
  ${ASIO_HEADER}
  "include/"
  "../include/"
  )
