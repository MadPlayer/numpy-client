#pragma once

#include <string>
#include <memory>
#include <functional>
#include "asio.hpp"
#include "asio/error_code.hpp"


class session
{
  using task = std::function<void(asio::ip::tcp::socket &)>;
  std::unique_ptr<asio::io_context> context_ptr_;
  std::unique_ptr<asio::ip::tcp::socket> socket_ptr_;
  task task_;

public:
  session();
  session(asio::ip::tcp::endpoint &ep);
  session(std::string address_v4, unsigned int port);
  session(session&& s);
  void connect(asio::ip::tcp::endpoint &ep);
  void assign_task(task& t);
  void assign_task(task&& t);
  void run();
  void reset();
};
