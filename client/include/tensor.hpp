#pragma once

#include "asio.hpp"
#include "packet.pb.h"
#include "message.hpp"
#include "blob.hpp"


namespace tensor
{
  using namespace asio;
  using namespace asio::ip;
  using elem_type = decltype(*std::declval<body::tensor>().mutable_data()->mutable_data());

  void send_tensor(tcp::socket& s, body::tensor& t);
  void get_tensor(tcp::socket& s, body::tensor & t);

  void
  operator >> (post::message& msg, body::tensor & t);

  void
  operator << (post::message& msg, body::tensor& t);

  template <typename T = elem_type>
  blob::blob<T> get_blob(body::tensor& t)
  {
    std::size_t length = t.channel() * t.height() * t.width();

    return {t.mutable_data()->mutable_data(), length, blob::element_number{}};
  }
}    
