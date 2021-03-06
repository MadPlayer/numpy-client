#pragma once

#include "onnxruntime_cxx_api.h"
#include "opencv2/core/mat.hpp"
#include "blob.hpp"
#include "tensor.hpp"
#include <vector>
#include <numeric>

class onnx_string
{
  const char *c_str_ = nullptr;
  Ort::AllocatorWithDefaultOptions allocator_;
public:
  onnx_string()
  {
  }

  onnx_string(char *c_str, Ort::AllocatorWithDefaultOptions& allocator):
    c_str_(c_str), allocator_(allocator)
  {
  }

  void operator = (onnx_string&& os)
  {
    if (c_str_)
      {
        allocator_.Free(reinterpret_cast<void*>(const_cast<char *>(c_str_)));
        c_str_ = nullptr;
      }
    c_str_ = os.c_str();
    os.c_str_ = nullptr;
  }

  ~onnx_string()
  {
    if (c_str_)
      allocator_.Free(reinterpret_cast<void*>(const_cast<char *>(c_str_)));
  }

  const char * c_str() const
  {
    return c_str_;
  }
};

class model
{
public:
  model(Ort::Env& env, const char *model_path, Ort::SessionOptions &option);

  // inference only continuous opencv matrix
  void inference(cv::Mat preprocessed, body::tensor& output);

private:
  Ort::Session session_;
  Ort::MemoryInfo allocator_info_;
  int64_t input_size_;
  int64_t output_size_;
  std::vector<int64_t> input_shape_;
  std::vector<int64_t> output_shape_;
  onnx_string input_name_;
  onnx_string output_name_;
};
