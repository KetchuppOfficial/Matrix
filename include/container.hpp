#ifndef INCLUDE_CONTAINER_HPP
#define INCLUDE_CONTAINER_HPP

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <iterator>
#include <utility>

namespace yLab
{

template<typename T>
class Buffer
{
public:

    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = T *;
    using size_type = std::size_t;

    Buffer(size_type count)
        : data_{count == 0 ? nullptr : static_cast<T *>(::operator new(count * sizeof(T)))},
          capacity_{count} {}

    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    Buffer(Buffer &&rhs) noexcept
        : data_{std::exchange(rhs.data_, nullptr)},
          capacity_{std::exchange(rhs.capacity_, 0)},
          size_{std::exchange(rhs.size_, 0)} {}

    Buffer &operator=(Buffer &&rhs) noexcept
    {
        swap(rhs);
        return *this;
    }

    void swap(Buffer &rhs) noexcept
    {
        std::swap(data_, rhs.data_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(size_, rhs.size_);
    }

protected:

    ~Buffer()
    {
        std::destroy(data_, data_ + size_);
        ::operator delete(data_);
    }

    T *data_;
    size_type capacity_;
    size_type size_ = 0;
};

template<typename T>
class Array : private Buffer<T> {
    using Buffer<T>::data_;
    using Buffer<T>::size_;
    using Buffer<T>::capacity_;

public:
    using typename Buffer<T>::value_type;
    using typename Buffer<T>::reference;
    using typename Buffer<T>::const_reference;
    using typename Buffer<T>::pointer;
    using typename Buffer<T>::const_pointer;
    using typename Buffer<T>::size_type;

    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using Buffer<T>::swap;

    template<std::forward_iterator It>
    Array(It first, It last) : Buffer<T>{static_cast<size_type>(std::distance(first, last))}
    {
        for (; first != last; ++first, ++size_)
            std::construct_at(data_ + size_, *first);
    }

    Array(std::initializer_list<T> ilist) : Array(ilist.begin(), ilist.end()) {}

    Array(size_type count) : Buffer<T>{count}
    {
        for (; size_ != count; ++size_)
            std::construct_at(data_ + size_, T{});
    }

    Array(size_type count, const value_type &value) : Buffer<T>{count}
    {
        for (; size_ != count; ++size_)
            std::construct_at(data_ + size_, value);
    }

    Array(const Array &rhs) : Buffer<T>{rhs.capacity_} {
        for (; size_ != capacity_; ++size_)
            std::construct_at(data_ + size_, rhs.data_[size_]);
    }

    Array &operator=(const Array &rhs) {
        Array tmp{rhs};
        swap(tmp);
        return *this;
    }

    const T *data() const noexcept { return data_; }
    T *data() noexcept { return data_; }

    size_type size() const noexcept { return size_; }

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return begin(); }

    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { end(); }

    reverse_iterator rbegin() noexcept { return reverse_iterator{begin()}; }
    const_reverse_iterator rbegin() const noexcept { return std::reverse_iterator{begin()}; }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    reverse_iterator rend() noexcept { return reverse_iterator{end()}; }
    const_iterator rend() const noexcept { return reverse_iterator{end()}; }
    const_iterator crend() const noexcept { rend(); }
};

} // namespace yLab

#endif // INCLUDE_CONTAINER_HPP
