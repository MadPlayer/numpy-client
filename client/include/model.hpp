#pragma once

#include "onnxruntime_cxx_api.h"
#include "opencv2/core/mat.hpp"
#include <vector>
#include <numeric>

class model
{
public:
  model(Ort::Env& env, const char *model_path, Ort::SessionOptions &option);

  // very dangerous (can dereference invalied memory)
  std::vector<float> & inference(std::vector<float> &input_data);

  // very dangerous (can dereference invalied memory)
  std::vector<float> & inference(cv::Mat& preprocessed);

private:
  Ort::Session session_;
  Ort::Value input_tensor_{nullptr};
  Ort::Value output_tensor_{nullptr};
  int64_t input_size_;
  std::vector<int64_t> input_shape_;
  std::vector<int64_t> output_shape_;
  Ort::MemoryInfo allocator_info_;
  std::vector<float> result_;
};
