#ifndef INCLUDE_ITERATOR_HPP
#define INCLUDE_ITERATOR_HPP

namespace yLab
{

template<typename Ptr_T>
class iterator final
{
    Ptr_T ptr_;

public:

    using iterator_category = typename std::random_access_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = std::remove_pointer_t<Ptr_T>;
    using reference = value_type&;
    using pointer = Ptr_T;

    iterator () = default;
    iterator (pointer ptr) : ptr_{ptr} {}

    iterator (const iterator &iter) = default;
    iterator &operator= (const iterator &iter) = default;

    iterator (iterator &&iter) = default;
    iterator &operator= (iterator &&iter) = default;

    const value_type *base () const { return ptr_; }

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

template<typename T>
iterator<T> operator+ (typename iterator<T>::difference_type n, const iterator<T> &iter)
{
    return iterator<T>{iter.base() + n};
}

template<typename T>
iterator<T>::difference_type operator- (const iterator<T> &lhs, const iterator<T> &rhs)
{
    return lhs.base() - rhs.base();
}

} // namespace yLab

#endif // INCLUDE_ITERATOR_HPP
