#pragma once

#include "scrfd.h"
#include "face_align.h"

namespace scrfd
{
  std::vector<cv::Mat> crop_faces(cv::Mat frame);
}
