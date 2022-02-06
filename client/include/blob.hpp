#pragma once
#include <cstddef>


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

  public:
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

    T* data()
    {
      return data_;
    }

    std::size_t size()
    {
      return byte_size_ * sizeof(T);
    }

    std::size_t byte_length()
    {
      return byte_size_;
    }
  
  };
}
