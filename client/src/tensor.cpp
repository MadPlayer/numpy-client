#include "tensor.hpp"

namespace tensor
{
  void send_tensor(tcp::socket& s, body::tensor& t)
  {
    // XXX: Hard Coding Server Directory!!!
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

  void
  operator >> (post::message& msg, body::tensor & t)
  {
    auto& stream = msg.get_stream();
    t.ParseFromIstream(&stream);
  }

  void
  operator << (post::message& msg, body::tensor& t)
  {
    auto& stream = msg.get_stream();
    t.SerializeToOstream(&stream);
  }

  blob::blob<elem_type> get_blob(body::tensor& t)
  {
    std::size_t length = t.channel() * t.height() * t.width();

    assert(length == t.data_size());

    return {t.mutable_data()->mutable_data(), length, blob::element_number{}};
  }
}
