#include "tensor.hpp"

namespace tensor
{
  void send_tensor(tcp::socket& s, body::tensor& t)
  {
    post::message msg("/numpy/inference");

    msg << t;

    msg.send(s);
  }

  void get_tensor(tcp::socket& s, body::tensor & t)
  {
    post::message msg("");

    msg.receive(s);

    msg >> t;
  }

  std::istream &
  operator >> (post::message& msg, body::tensor & t)
  {
    auto& stream = msg.get_stream();
    t.ParseFromIstream(&stream);

    return stream;
  }

  std::ostream &
  operator << (post::message& msg, body::tensor& t)
  {
    auto& stream = msg.get_stream();
    t.SerializeToOstream(&stream);

    return stream;
  }
}    

