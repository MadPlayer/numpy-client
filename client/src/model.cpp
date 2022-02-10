#include "model.hpp"


model::model(Ort::Env& env, const char *model_path, Ort::SessionOptions &option):
  session_(env, model_path, option),
  allocator_info_(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault))
{
  input_shape_ = session_.GetInputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();
  output_shape_ = session_.GetOutputTypeInfo(0).GetTensorTypeAndShapeInfo().GetShape();

  input_size_ = std::accumulate(input_shape_.begin(), input_shape_.end(), 1, std::multiplies<int64_t>{});
  auto output_size = std::accumulate(output_shape_.begin(), output_shape_.end(), 1, std::multiplies<int64_t>{});

  result_.resize(output_size);

  output_tensor_ = Ort::Value::CreateTensor<float>(allocator_info_, result_.data(), result_.size(),
                                                   output_shape_.data(), output_shape_.size());
}

// very dangerous (can dereference invalied memory)
std::vector<float> &
model::inference(std::vector<float> &input_data)
{
  Ort::AllocatorWithDefaultOptions allocator;
  const char *input_name = session_.GetInputName(0, allocator);
  const char *output_name = session_.GetOutputName(0, allocator);
  const char *input_names[] = {input_name};
  const char *output_names[] = {output_name};

  input_tensor_ = Ort::Value::CreateTensor<float>(allocator_info_, input_data.data(), input_size_,
                                                  input_shape_.data(), input_shape_.size());
  session_.Run(Ort::RunOptions{nullptr}, input_names, &input_tensor_, 1, output_names, &output_tensor_, 1);

  allocator.Free((void *)input_name);
  allocator.Free((void *)output_name);

  return result_;
}

// very dangerous (can dereference invalied memory)
std::vector<float> &
model::inference(cv::Mat& preprocessed)
{
  Ort::AllocatorWithDefaultOptions allocator;
  const char *input_name = session_.GetInputName(0, allocator);
  const char *output_name = session_.GetOutputName(0, allocator);
  const char *input_names[] = {input_name};
  const char *output_names[] = {output_name};

  input_tensor_ = Ort::Value::CreateTensor<float>(allocator_info_, reinterpret_cast<float *>(preprocessed.data), input_size_,
                                                  input_shape_.data(), input_shape_.size());
  session_.Run(Ort::RunOptions{nullptr}, input_names, &input_tensor_, 1, output_names, &output_tensor_, 1);

  allocator.Free((void *)input_name);
  allocator.Free((void *)output_name);

  return result_;
}
