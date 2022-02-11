#include "camera.hpp"
#include "opencv2/videoio.hpp"
#include <exception>
#include <iostream>
#include <system_error>


namespace camera
{
  constexpr char GSTREAMER_PIPELINE[] =
    "libcamerasrc ! video/x-raw, width=1280,height=720, framerate=30/1 !"
    "videoconvert ! videoscale ! appsink";

  cv::VideoCapture create_camera()
  {
    cv::VideoCapture camera;

    camera.open(GSTREAMER_PIPELINE, cv::CAP_GSTREAMER);
    if (camera.isOpened())
      {
        return camera;
      }

    std::cerr << "NO Available GSTREAMER\n";
    if (camera.open(0))
      {
        std::cout << "use default camera device\n";
        camera.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        camera.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
        return camera;
      }
    std::cerr << "camera initialize failed\n";
    return camera;
  }
}    
