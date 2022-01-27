#pragma once

#include <iostream>
#include <string>
#include "asio.hpp"

namespace client_message
{
  class post_message
  {
    std::string header_;
    asio::streambuf buffer_;
    std::iostream data_stream_;
    bool invalied_;

  public:
    post_message(std::string dir);
    void send(asio::ip::tcp::socket &s) throw();
    std::size_t receive(asio::ip::tcp::socket &s) throw();
    std::iostream & get_stream();
  };

}
