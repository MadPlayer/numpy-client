#pragma once

#include "asio.hpp"
#include "packet.pb.h"
#include "message.hpp"
#include "blob.hpp"

#include <tuple>

namespace tensor
{
  using namespace asio;
  using namespace asio::ip;
  using elem_type = std::remove_pointer_t<
    decltype(std::declval<body::tensor>().mutable_data()->mutable_data())
    >;
  using blob = blob::blob<elem_type>;

  // shape format is CHW.
  using shape = std::tuple<std::size_t, std::size_t, std::size_t>;

  // send and receive tensor by socket
  void send_tensor(tcp::socket& s, body::tensor& t);
  void get_tensor(tcp::socket& s, body::tensor & t);

  // extract tensor from message
  void
  operator >> (post::message& msg, body::tensor & t);

  // write tensor into message body
  void
  operator << (post::message& msg, body::tensor& t);

  // init tensor to shape. every data tensor contains will be erased.
  void init_tensor(body::tensor& t, shape tensor_shape);
}

namespace blob
{
  // get tensor data blob from tensor
  template<>
  void get_blob(tensor::blob& b, body::tensor& t);
}
