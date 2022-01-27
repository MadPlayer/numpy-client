#include "asio.hpp"
#include <iostream>
#include <string>
#include <system_error>
#include "packet.pb.h"

using namespace asio;
using namespace asio::ip;

int main(int argc, char *argv[])
{
  std::string address = "127.0.0.1";
  io_context context;
  tcp::endpoint ep(make_address(address), 5000);


  try
    {
      tcp::socket socket(context, tcp::v4());

      socket.connect(ep);
      std::string msg =
        "POST / HTTP/1.0\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "hello";
      write(socket, buffer(msg));

      streambuf buf;
      std::istream input(&buf);

      read_until(socket, buf, '\n');

      input >> msg;
      std::cout << msg << std::endl;

      socket.close();
    }
  catch (std::system_error &ec)
    {
      std::cerr << "Error(" << ec.code() << "): " << ec.what() << std::endl;
      return ec.code().value();
    }


  return 0;
}
