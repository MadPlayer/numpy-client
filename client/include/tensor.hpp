#pragma once

#include "asio.hpp"
#include "packet.pb.h"
#include "message.hpp"


namespace tensor
{
  using namespace asio;
  using namespace asio::ip;

  void send_tensor(tcp::socket& s, body::tensor& t);
  void get_tensor(tcp::socket& s, body::tensor & t);

  std::istream &
  operator >> (post::message& msg, body::tensor & t);

  std::ostream &
  operator << (post::message& msg, body::tensor& t);

}    
