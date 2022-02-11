#pragma once

#include "asio.hpp"
#include <iostream>
#include <string>
#include <system_error>
#include <chrono>
#include "tensor.hpp"
#include "session.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "onnxruntime_cxx_api.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/dnn/dnn.hpp"


#include "packet.pb.h"
