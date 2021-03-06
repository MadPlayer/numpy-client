#include "client.hpp"
#include "message.hpp"
#include <string>

using namespace asio;
using namespace asio::ip;


std::ostream &
operator <<(std::ostream& o, const body::tensor &t)
{
  
  o << "shape : \n [";
  for (auto &i: t.shape())
    {
      o << i << ", ";
    }
  o<< "]\n";

  o << "tensor : \n [";
  for (auto &i: t.data())
    {
      o << i << ", ";
    }
  o << "]\n";

  return o;
}

// from https://github.com/leimao/ONNX-Runtime-Inference
cv::Mat preprocess(cv::Mat origin_bgr, std::tuple<int, int> shape)
{
  cv::Mat resizedImageBGR, resizedImageRGB, resizedImage, preprocessedImage;
  cv::resize(origin_bgr, resizedImageBGR,
             cv::Size(std::get<0>(shape), std::get<1>(shape)),
             cv::InterpolationFlags::INTER_CUBIC);
  cv::cvtColor(resizedImageBGR, resizedImageRGB,
               cv::ColorConversionCodes::COLOR_BGR2RGB);
  resizedImageRGB.convertTo(resizedImage, CV_32F, 1.0 / 255);

  cv::Mat channels[3];
  cv::split(resizedImage, channels);
  // Normalization per channel
  // Normalization parameters obtained from
  // https://github.com/onnx/models/tree/master/vision/classification/squeezenet
  channels[0] = (channels[0] - 0.485) / 0.229;
  channels[1] = (channels[1] - 0.456) / 0.224;
  channels[2] = (channels[2] - 0.406) / 0.225;
  cv::merge(channels, 3, resizedImage);
  // HWC to CHW
  cv::dnn::blobFromImage(resizedImage, preprocessedImage);

  return preprocessedImage;
}

int main(int argc, char *argv[])
{
  const std::string address = "127.0.0.1";
  io_context context;
  tcp::endpoint ep(make_address(address), 5000);

  auto cam = camera::create_camera();
  cv::Mat img;

  // onnxruntime settings
  Ort::Env env{ORT_LOGGING_LEVEL_ERROR, "client prototype"};
  Ort::SessionOptions session_options;
  session_options.SetGraphOptimizationLevel(ORT_ENABLE_ALL)
    .SetExecutionMode(ORT_SEQUENTIAL);

  model resnet18{env, argv[1], session_options};

  body::batched_tensor ts;

  while(true)
    {
      cam >> img;
      try
        {
          session s(ep);  // create session and connect to endpoint

          auto faces = scrfd::crop_faces(img);

          for (auto &face : faces)
            {
              // XXX: hard coding input image size
              auto preprocessed = preprocess(face, {224, 224});
              auto *t = ts.add_tensors(); // allocate new tensor
              resnet18.inference(preprocessed, *t);
            }

          s.assign_task([&ts](tcp::socket &s){
            tensor::send_tensors(s, ts);

            post::message msg("");
            msg.receive(s);

            std::string content;
            msg.get_stream() >> content;
            std::cout << content << std::endl;
          });

          s.run();
        
          ts.clear_tensors();

          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
      catch (std::system_error &ec)
        {
          std::cerr << "Error(" << ec.code() << "): " << ec.what() << std::endl;
          return ec.code().value();
        }
    }

  return 0;
}
