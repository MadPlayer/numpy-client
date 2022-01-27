#include "asio.hpp"
#include <iostream>
#include <string>
#include <system_error>
#include "asio/error_code.hpp"
#include "packet.pb.h"

using namespace asio;
using namespace asio::ip;


void
send_tensor(tcp::socket& s, study::tensor& t)
{
  std::string msg = "POST /numpy/inference HTTP/1.0\r\n"
    "Content-Type: application/octet-stream\r\n"
    "Content-Length: ";

  std::string data;
  t.SerializePartialToString(&data);

  msg += std::to_string(data.length());
  std::cout << msg << std::endl;
  msg += "\r\n\r\n";

  write(s, buffer(msg));
  write(s, buffer(data));
}

void
get_tensor(tcp::socket& s, study::tensor &t)
{
  error_code ec;
  std::string header;
  streambuf buf;
  std::istream input(&buf);
  buf.prepare((1024 * 1024) << 2);

  t.clear_channel();
  t.clear_data();
  t.clear_height();
  t.clear_width();

  read(s, buf, ec);
  size_t bytes = 0;
  while (bytes != 1)
    {
      std::getline(input, header);
      std::cout << header << std::endl;
      bytes = header.length();
    }

  t.ParseFromIstream(&input);
}

std::ostream &
operator <<(std::ostream& o, const study::tensor &t)
{
  o << "width: " << t.width() << std::endl
    << "height: " << t.height() << std::endl
    << "channel: " << t.channel() << std::endl;
  o << "tensor : \n [";
  for (auto &i: t.data())
    {
      o << i << ", ";
    }
  o << "]\n";

  return o;
}

int main(int argc, char *argv[])
{
  std::string address = "127.0.0.1";
  io_context context;
  tcp::endpoint ep(make_address(address), 5000);


  try
    {
      tcp::socket socket(context, tcp::v4());

      socket.connect(ep);

      study::tensor t;
      t.set_width(1);
      t.set_height(2);
      t.set_channel(33);
      t.mutable_data()->Resize(10, 1);

      send_tensor(socket, t);

      get_tensor(socket, t);

      std::cout << t;
    }
  catch (std::system_error &ec)
    {
      std::cerr << "Error(" << ec.code() << "): " << ec.what() << std::endl;
      return ec.code().value();
    }

  return 0;
}
