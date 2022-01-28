#include "session.hpp"
#include "asio/error_code.hpp"
#include "asio/ip/address.hpp"
#include <memory>
#include <utility>

using namespace asio;
using namespace asio::ip;

session::session()
{
  context_ptr_ = std::make_unique<io_context>();
  socket_ptr_ = std::make_unique<tcp::socket>(*context_ptr_, tcp::v4());
}

session::session(tcp::endpoint &ep):
  session()
{
  socket_ptr_->connect(ep);
}

session::session(std::string address_v4, unsigned int port):
  session()
{
  tcp::endpoint ep(make_address(address_v4), port);
  socket_ptr_->connect(ep);
}

session::session(session&& s)
{
  context_ptr_ = std::move(s.context_ptr_);
  socket_ptr_ = std::move(s.socket_ptr_);
  task_ = s.task_;
}

void session::connect(tcp::endpoint &ep)
{
  socket_ptr_->connect(ep);
}

void session::assign_task(task&& t)
{
}

void session::run()
{
}
