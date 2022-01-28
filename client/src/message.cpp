#include "message.hpp"
#include "asio/error.hpp"
#include "asio/execution_context.hpp"

using namespace asio;
using namespace asio::ip;
using namespace post;


message::message(std::string dir):
  data_stream_(&buffer_)
{
  header_ = "POST " + dir + " HTTP/1.0\r\n"
    "Content-Type: application/octet-stream\r\n"
    "Content-Length: ";

  // message has no data yet
  invalied_ = true;
}

void message::send(tcp::socket &s) throw()
{
  header_ += std::to_string(buffer_.size()) + "\r\n\r\n";

  write(s, buffer(header_));
  write(s, buffer_);
  invalied_ = true;
}

std::size_t
message::receive(tcp::socket &s) throw()
{
  std::string header ;
  error_code ec;
  size_t total_bytes = 0;

  buffer_.prepare(1024*1024*8);
  read(s, buffer_, ec);

  // get rid of HTTP header
  size_t bytes = 0;
  while (bytes != 1)
    {
      std::getline(data_stream_, header);
      bytes = header.length();
      total_bytes += bytes;
    }
  invalied_ = false;
  return total_bytes;
}

std::iostream &
message::get_stream()
{
  return data_stream_;
}
