#include "asio.hpp"
#include <iostream>
#include <string>
#include <system_error>
#include "message.hpp"
#include "tensor.hpp"
#include "packet.pb.h"

using namespace asio;
using namespace asio::ip;


std::ostream &
operator <<(std::ostream& o, const body::tensor &t)
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

      body::tensor t;
      t.set_width(1);
      t.set_height(2);
      t.set_channel(33);
      t.mutable_data()->Resize(10, 1);

      tensor::send_tensor(socket, t);

      tensor::get_tensor(socket, t);

      std::cout << t;
    }
  catch (std::system_error &ec)
    {
      std::cerr << "Error(" << ec.code() << "): " << ec.what() << std::endl;
      return ec.code().value();
    }

  return 0;
}
