#ifndef INCLUDE_CONTAINER_HPP
#define INCLUDE_CONTAINER_HPP

#include <memory> // for std::adressof
#include <algorithm> // for std::swap

#include "iterator.hpp"

namespace Containers
{

template <typename T>
void construct (T *ptr, const T &value) { new (ptr) T{value}; }

template <typename T>
void construct (T *ptr, T &&value) { new (ptr) T{std::move (value)}; }

template <typename T>
void destroy (T *ptr) noexcept { ptr->~T(); }

template <typename Fwd_Iter>
void destroy (Fwd_Iter begin, Fwd_Iter end) noexcept
{
    while (begin != end)
        destroy (std::addressof (*begin++));
}

template <typename T>
class Array_Buff
{
protected:

    T *data_;
    size_t size_;

    Array_Buff (const size_t size = 0)
          : data_{(size == 0) ? nullptr : static_cast<T *>(::operator new (sizeof (T) * size))},
            size_{size}
    {}

    Array_Buff (const Array_Buff &rhs) = delete;
    Array_Buff &operator= (const Array_Buff &rhs) = delete;
    
    Array_Buff (Array_Buff &&rhs) noexcept
               : data_{std::exchange (rhs.data_, nullptr)}, 
                 size_{std::move (rhs.size_)}
    {
        rhs.size_ = 0;
    }

    Array_Buff &operator= (Array_Buff &&rhs) noexcept
    {
        std::swap (data_, rhs.data_);
        std::swap (size_, rhs.size_);

        return *this;
    }

    virtual ~Array_Buff ()
    {
        destroy (data_, data_ + size_);
        ::operator delete (data_);
    }
};

template <typename T>
struct Array : private Array_Buff<T>
{   
    using iterator = typename yLab::iterator<T>;
    using const_iterator = typename yLab::const_iterator<T>;
    
    Array (const size_t size = 0) : Array_Buff<T> {size} {}

    Array (const Array &rhs) : Array_Buff<T>{rhs.size_}
    {
        for (auto i = 0; i < rhs.size_; i++)
            construct (this->data_ + i, rhs.data_[i]);
    }

    Array &operator= (const Array &rhs)
    {
        Array tmp {rhs};
        std::swap (*this, tmp);

        return *this;
    }
    
    Array (Array &&rhs) = default;
    Array &operator= (Array &&rhs) = default;

    size_t size () const { return this->size_; }

    const T *data () const & { return this->data_; }
    T *data () & { return this->data_; }

    const T &operator[] (const size_t i) const { return this->data_[i]; }
    T &operator[] (const size_t i) { return this->data_[i]; }

    auto begin () { return iterator{data()}; }
    auto begin () const { return const_iterator{data()}; }
    auto cbegin () const { return const_iterator {data()}; }

    auto end () { return iterator{data() + size()}; }
    auto end () const { return const_iterator{data() + size()}; }
    auto cend () const { return const_iterator{data() + size()}; }
};

} // namespace Containers

#endif // INCLUDE_CONTAINER_HPP
