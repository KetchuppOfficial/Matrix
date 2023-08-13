#ifndef INCLUDE_CONTAINER_HPP
#define INCLUDE_CONTAINER_HPP

#include <algorithm>
#include <utility>

#include "iterator.hpp"

namespace yLab
{

template<typename T>
class Array_Buff
{
protected:

    T *data_;
    std::size_t capacity_;

    Array_Buff (std::size_t count = 0)
               : data_{(count == 0) ? nullptr
                                    : static_cast<T *>(::operator new (sizeof (T) * count))},
                 capacity_{count} {}

    Array_Buff (const Array_Buff &rhs) = delete;
    Array_Buff &operator= (const Array_Buff &rhs) = delete;

    Array_Buff (Array_Buff &&rhs) noexcept
               : data_{std::exchange (rhs.data_, nullptr)},
                 capacity_{std::exchange (rhs.capacity_, 0)} {}

    Array_Buff &operator= (Array_Buff &&rhs) noexcept
    {
        std::swap (data_, rhs.data_);
        std::swap (capacity_, rhs.capacity_);

        return *this;
    }

    ~Array_Buff ()
    {
        ::operator delete (data_);
    }
};

template<typename T>
class Array final: private Array_Buff<T>
{
public:

    using iterator = typename yLab::iterator<T>;
    using const_iterator = typename yLab::const_iterator<T>;

private:

    std::size_t size_ = 0;

public:

    Array (std::size_t count = 0) : Array_Buff<T>{count}
    {
        for (auto i = 0; i != count; ++i, ++size_)
            std::construct_at (this->data_ + i, T{});
    }

    Array (const Array &rhs) : Array_Buff<T>{rhs.capacity_}
    {
        for (auto i = 0; i != rhs.capacity_; ++i, ++size_)
            std::construct_at (this->data_ + i, rhs.data_[i]);
    }

    Array &operator= (const Array &rhs)
    {
        Array tmp{rhs};
        std::swap (*this, tmp);

        return *this;
    }

    ~Array ()
    {
        std::destroy (this->data_, this->data_ + size_);
    }

    Array (Array &&rhs) = default;
    Array &operator= (Array &&rhs) = default;

    std::size_t size () const { return this->capacity_; }

    const T *data () const & { return this->data_; }
    T *data () & { return this->data_; }

    const T &operator[] (std::size_t i) const & { return this->data_[i]; }
    T &operator[] (std::size_t i) & { return this->data_[i]; }

    iterator begin () { return iterator{data()}; }
    const_iterator begin () const { return const_iterator{data()}; }
    const_iterator cbegin () const { return begin(); }

    iterator end () { return iterator{data() + size()}; }
    const_iterator end () const { return const_iterator{data() + size()}; }
    const_iterator cend () const { return end(); }
};

} // namespace yLab

#endif // INCLUDE_CONTAINER_HPP
