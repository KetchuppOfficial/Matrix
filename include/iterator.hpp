#ifndef INCLUDE_ITERATOR_HPP
#define INCLUDE_ITERATOR_HPP

namespace yLab
{

template <typename T>
class iterator final
{
    T *ptr_;

public:

    using iterator_category = typename std::random_access_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using reference = T&;
    using pointer = T*;

    iterator () = default;
    iterator (T *ptr) : ptr_{ptr} {}

    iterator (const iterator &iter) = default;
    iterator &operator= (const iterator &iter) = default;

    iterator (iterator &&iter) = default;
    iterator &operator= (iterator &&iter) = default;

    const T *base () const { return ptr_; }

    iterator &operator+= (const difference_type offset) { return (ptr_ + offset, *this); }
    iterator &operator-= (const difference_type offset) { return (ptr_ - offset, *this); }

    iterator operator+ (const difference_type n) const { return iterator{ptr_ + n}; }
    iterator operator- (const difference_type n) const { return iterator{ptr_ - n}; }

    reference operator* () const { return *ptr_; }
    pointer operator-> () const { return ptr_; }

    reference operator[] (const difference_type offset) const { return ptr_[offset]; }

    iterator &operator++ () { return (++ptr_, *this); }
    iterator operator++ (int) { return iterator{ptr_++}; }

    iterator &operator-- () { return (--ptr_, *this); }
    iterator operator-- (int) { return iterator{ptr_--}; }

    auto operator<=> (const iterator &iter) const = default;
};

template <typename T>
inline iterator<T> operator+ (typename iterator<T>::difference_type n, const iterator<T> &iter)
{
    return iterator<T>{iter.base() + n};
}

template <typename T>
inline iterator<T>::difference_type operator- (const iterator<T> &lhs, const iterator<T> &rhs)
{
    return lhs.base() - rhs.base();
}

template <typename T>
class const_iterator final
{
    const T *ptr_;

public:

    using iterator_category = typename std::random_access_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = const T;
    using reference = const T&;
    using pointer = const T*;

    const_iterator () = default;
    const_iterator (const T *ptr) : ptr_{ptr} {}

    const_iterator (const const_iterator &iter) = default;
    const_iterator &operator= (const const_iterator &iter) = default;

    const_iterator (const_iterator &&iter) = default;
    const_iterator &operator= (const_iterator &&iter) = default;

    pointer base () const { return ptr_; }

    const_iterator &operator+= (const difference_type offset) { return (ptr_ + offset, *this); }
    const_iterator &operator-= (const difference_type offset) { return (ptr_ - offset, *this); }

    const_iterator operator+ (const difference_type n) const { return const_iterator{ptr_ + n}; }
    const_iterator operator- (const difference_type n) const { return const_iterator{ptr_ - n}; }

    reference operator* () const { return *ptr_; }
    pointer operator-> () const { return ptr_; }

    reference operator[] (const difference_type &offset) const { return ptr_[offset]; };

    const_iterator &operator++ () { return (ptr_++, *this); }
    const_iterator operator++ (int) { return const_iterator{ptr_++}; }

    const_iterator &operator-- () { return (ptr_--, *this); }
    const_iterator operator-- (int) { return const_iterator{ptr_--}; }

    auto operator<=> (const const_iterator &iter) const = default;
};

template <typename T>
const_iterator<T> operator+ (typename const_iterator<T>::difference_type n, const const_iterator<T> &iter)
{
    return const_iterator<T>{iter.base() + n};
}

template <typename T>
const_iterator<T>::difference_type operator- (const const_iterator<T> &lhs, const const_iterator<T> &rhs)
{
    return lhs.base() - rhs.base();
}

} // namespace yLab

#endif // INCLUDE_ITERATOR_HPP
