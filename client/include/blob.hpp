#pragma once
#include <cstddef>
#include <stdexcept>


namespace blob
{
  struct byte_size {};
  struct element_number {};

  // class blob does not own data.
  template <typename T>
  class blob
  {
    T* data_;
    std::size_t byte_size_;

    blob(T* data, std::size_t size, byte_size):
      data_(data), byte_size_(size)
    {
    }

    blob(T* data, std::size_t elem_num, element_number):
      data_(data), byte_size_(elem_num * sizeof(T))
    {
    }

    blob(T* data, std::size_t size):
      blob(data, size, byte_size{})
    {
    }

    blob(blob<T>& b):
      blob(b.data, b.size, element_number{})
    {
    }

    blob<T>& operator = (blob<T>&) = default;

    // use just default copy, cause blob doesn't own data.
    void operator = (blob<T>&& b)
    {
      data_ = b.data_;
      byte_size_ = b.byte_size_;
    }

  public:

    template<typename S>
    explicit blob(S& s):
      data_(nullptr), byte_size_(0)
    {
      throw std::runtime_error("Specialize blob constructor for your own type.");
    }

    T* data()
    {
      return data_;
    }

    std::size_t size()
    {
      return byte_size_ / sizeof(T);
    }

    std::size_t byte_length()
    {
      return byte_size_;
    }
  };

}

