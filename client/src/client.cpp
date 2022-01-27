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
      std::string msg = "POST /numpy/inference HTTP/1.0\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: ";

      study::tensor t;
      t.set_width(1);
      t.set_height(2);
      t.set_channel(33);
      t.mutable_data()->Resize(10, 1);

      std::string data;
      t.SerializePartialToString(&data);

      msg += std::to_string(data.length());
      std::cout << msg << std::endl;
      msg += "\r\n\r\n";

      write(socket, buffer(msg));
      write(socket, buffer(data));

    }
  catch (std::system_error &ec)
    {
      std::cerr << "Error(" << ec.code() << "): " << ec.what() << std::endl;
      return ec.code().value();
    }

  return 0;
}
