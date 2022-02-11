#include "tensor.hpp"

namespace tensor
{
  enum   // tensor shape is CHW.
    {
      CHANNEL,
      HEIGHT,
      WIDTH
    };

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

  void init_tensor(body::tensor& t, shape tensor_shape)
  {
    t.set_channel(std::get<CHANNEL>(tensor_shape));
    t.set_height(std::get<HEIGHT>(tensor_shape));
    t.set_width(std::get<WIDTH>(tensor_shape));

    std::size_t length = std::get<CHANNEL>(tensor_shape) * std::get<HEIGHT>(tensor_shape)
      * std::get<WIDTH>(tensor_shape);

    t.mutable_data()->Resize(length, 0); // initialize tensor with zero
  }

}

namespace blob
{
  template<>
  void get_blob(blob<tensor::elem_type>& b, body::tensor& t)
  {
    std::size_t length = t.channel() * t.height() * t.width();

    assert(length == t.data_size());

    b = {t.mutable_data()->mutable_data(), length, element_number{}};
  }
}
