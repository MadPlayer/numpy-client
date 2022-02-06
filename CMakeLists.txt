cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(numpy_sending_client VERSION 0.1)

include_directories(BEFORE
  ${ASIO_HEADER}
  ${CMAKE_CURRENT_BINARY_DIR}/include
  )

add_subdirectory(include)
add_subdirectory(client)

