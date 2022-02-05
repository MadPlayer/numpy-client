#pragma once

#include "asio.hpp"
#include "packet.pb.h"
#include "message.hpp"
#include "blob.hpp"


namespace tensor
{
  using namespace asio;
  using namespace asio::ip;
  using elem_type = std::remove_pointer_t<
    decltype(std::declval<body::tensor>().mutable_data()->mutable_data())
    >;

  void send_tensor(tcp::socket& s, body::tensor& t);
  void get_tensor(tcp::socket& s, body::tensor & t);

  void
  operator >> (post::message& msg, body::tensor & t);

  void
  operator << (post::message& msg, body::tensor& t);

  // get tensor data blob from tensor
  blob::blob<elem_type> get_blob(body::tensor& t);
}    
