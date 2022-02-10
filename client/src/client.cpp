#include "asio.hpp"
#include <iostream>
#include <string>
#include <system_error>
#include <chrono>
#include "tensor.hpp"
#include "session.hpp"
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


  while (true)
    {
      try
        {
          session s(ep);  // create session and connect to endpoint

          body::tensor t;
          tensor::init_tensor(t, {3, 2, 1});
          tensor::blob blob;
          blob::get_blob(blob, t);

          auto list = blob.data();
          for (int i = 0; i < blob.size(); i++)
            {
              list[i] = i*3;
            }

          s.assign_task([&t](tcp::socket &s){
            tensor::send_tensor(s, t);
            tensor::get_tensor(s, t);
          });

          s.run();
        
          std::cout << t;
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
      catch (std::system_error &ec)
        {
          std::cerr << "Error(" << ec.code() << "): " << ec.what() << std::endl;
          return ec.code().value();
        }
    }

  return 0;
}
