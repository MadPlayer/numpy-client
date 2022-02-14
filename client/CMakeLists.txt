function(library_check library_var library_name)
  if (library_var)
    message("-- Found ${library_name} " ${library_var})
  else()
    message(FATAL_ERROR
      "Can't found ${library_name}."
      "Need to define ${library_name}_DIR when you run cmake."
      )
  endif()
endfunction()

set(client_sources
  client.cpp
  message.cpp
  tensor.cpp
  session.cpp
  camera.cpp
  model.cpp
  )
list(TRANSFORM client_sources PREPEND "src/")

add_executable(client)

# requirements
find_package(Protobuf REQUIRED)
find_package(OpenCV 4.2 REQUIRED PATHS ${OpenCV4_DIR})
find_library(ONNXRuntime onnxruntime HINTS
  ENV LIBRARY_PATH              #gcc default lib path
  ${ONNXRUNTIME_DIR}/lib
  )

library_check(${ONNXRuntime} ONNXRuntime)

target_sources(client PRIVATE
  ${client_sources}
  )

target_link_libraries(client
  ${Protobuf_LIBRARY}
  ${ONNXRuntime}
  PACKET_LIB
  pthread
  opencv_core
  opencv_videoio
  opencv_dnn
  opencv_imgproc
  )

target_include_directories(client PRIVATE
  include
  PUBLIC
  ${OpenCV_INCLUDE_DIR}/opencv4 #default header location.
  ${ONNXRUNTIME_DIR}/include
  )
