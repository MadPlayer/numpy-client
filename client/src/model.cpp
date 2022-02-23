#include "model.hpp"
#include <iostream>

model::model(Ort::Env& env, const char *model_path, Ort::SessionOptions &option):
  session_(env, model_path, option),
  allocator_info_(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault))
{
  input_shape_ = session_.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
  output_shape_ = session_.GetOutputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();

  Ort::AllocatorWithDefaultOptions allocator;
  input_name_ = { session_.GetInputName(0, allocator), allocator };
  output_name_ = { session_.GetOutputName(0, allocator), allocator };

  input_size_ = std::accumulate(input_shape_.begin(), input_shape_.end(), 1, std::multiplies<int64_t>{});
  output_size_ = std::accumulate(output_shape_.begin(), output_shape_.end(), 1, std::multiplies<int64_t>{});
}

void model::inference(cv::Mat preprocessed, body::tensor& output)
{
  const char *input_names[] = {input_name_.c_str()};
  const char *output_names[] = {output_name_.c_str()};

  if (preprocessed.isContinuous())
    {
      auto input_tensor = Ort::Value::CreateTensor<float>(allocator_info_, reinterpret_cast<float *>(preprocessed.data),
                                                          input_size_, input_shape_.data(), input_shape_.size());

      tensor::init_tensor(output, output_shape_);

      tensor::blob blob(output);

      auto output_tensor = Ort::Value::CreateTensor<float>(allocator_info_, blob.data(), blob.size(),
                                                           output_shape_.data(), output_shape_.size());

      session_.Run(Ort::RunOptions{nullptr}, input_names, &input_tensor, 1, output_names, &output_tensor, 1);
    }
  else
    {
      std::cerr << "preprocessed image is not continuous\n";
    }
}
