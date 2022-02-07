set(client_sources
  client.cpp
  message.cpp
  tensor.cpp
  session.cpp
  )
list(TRANSFORM client_sources PREPEND "src/")

add_executable(client)

# requirements
find_package(Protobuf REQUIRED)
find_library(ONNXRuntime onnxruntime HINTS
  ENV LIBRARY_PATH              #gcc default lib path
  ${ONNXRUNTIME_DIR}/lib
  )

if (ONNXRuntime)
  message("Found ONNX Runtime " ${ONNXRuntime})
else()
  message(FATAL_ERROR
    "Can't found ONNX Runtime."
    "Need to define ONNXRUNTIME_DIR when you run cmake."
    )
endif()

target_sources(client PRIVATE
  ${client_sources}
  )

target_link_libraries(client
  ${Protobuf_LIBRARY}
  ${ONNXRuntime}
  PACKET_LIB
  pthread
  )

target_include_directories(client PRIVATE
  include
  PUBLIC
  ${ONNXRUNTIME_DIR}/include
  )
