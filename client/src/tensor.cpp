#include "tensor.hpp"
#include "message.hpp"
#include <numeric>

// tensor shape is BCHW
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

  void init_tensor(body::tensor& t, const std::vector<int64_t>& shape)
  {
    *t.mutable_shape() = {shape.begin(), shape.end()};

    const std::size_t length = std::accumulate(begin(shape), end(shape), 1, std::multiplies<int64_t>{});

    t.mutable_data()->Resize(length, 0); // initialize tensor with zero
  }

  // BATCHED TENSOR ////////////////////////////////////
  void send_tensors(tcp::socket& s, body::batched_tensor& ts)
  {
    // XXX: Hard Coding Server Directory!!!
    post::message msg("/numpy/inference/batched");

    msg << ts;

    msg.send(s);
  }

  void get_tensors(tcp::socket& s, body::batched_tensor& ts)
  {
    post::message msg("");

    msg.receive(s);

    msg >> ts;
  }

  void
  operator >> (post::message& msg, body::batched_tensor& ts)
  {
    auto& stream = msg.get_stream();
    ts.ParseFromIstream(&stream);
  }

  void
  operator << (post::message& msg, body::batched_tensor& ts)
  {
    auto& stream = msg.get_stream();
    ts.SerializeToOstream(&stream);
  }

}

namespace blob
{
  template<>
  template<>
  blob<tensor::elem_type>::blob(body::tensor& t)
  {
    data_ = t.mutable_data()->mutable_data();
    byte_size_ = t.mutable_data()->size() * sizeof(tensor::elem_type);
  }

}
