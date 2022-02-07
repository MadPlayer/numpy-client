cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(numpy_sending_client VERSION 0.1)

if (NOT DEFINED ASIO_HEADER AND DEFINED ENV{ASIO_HEADER})
  set(ASIO_HEADER $ENV{ASIO_HEADER})
  message(${ASIO_HEADER})
else()
  message(FATAL_ERROR "Needs ASIO library")
endif()

include_directories(BEFORE
  ${ASIO_HEADER}
  ${CMAKE_CURRENT_BINARY_DIR}/include
  )

add_subdirectory(include)
add_subdirectory(client)

