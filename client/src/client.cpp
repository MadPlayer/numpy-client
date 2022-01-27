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

study::tensor
get_tensor(tcp::socket& s)
{
  study::tensor t;
  streambuf sbuf;
  std::istream input(&sbuf);
  size_t bytes = 0;

  try {
    bytes = read(s, sbuf);
  } catch (std::system_error &ec) {
    std::cerr << "bytes: " << bytes << std::endl;
  }
  sbuf.commit(bytes);
  std::string line;
  
  int length = 2;
  while (length != 1)
    {
      std::getline(input, line);
      std::cout << line;
      length = line.length();
    }
  t.ParseFromIstream(&input);

  return t;
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

      streambuf b;
      std::string tmp;
      std::istream input(&b);
      b.prepare(10000);
      error_code e;
      read(socket, b, e);
      while (!input.eof())
        {
          std::getline(input, tmp);
          std::cout << tmp << std::endl;
          if (tmp.length() == 1) // now start data
            {
              break;
            }
        }
      t.ParseFromIstream(&input);
      std::cout << t.width() << std::endl;
      std::cout << t.height() << std::endl;
      std::cout << t.channel() << std::endl;
      for (const auto i: t.data())
        {
          std::cout << i << std::endl;
        }

    }
  catch (std::system_error &ec)
    {
      std::cerr << "Error(" << ec.code() << "): " << ec.what() << std::endl;
      return ec.code().value();
    }

  return 0;
}
