#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include <type_traits>

namespace Matrix
{

template <typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
class Sq_Matrix
{
    using size_t = std::size_t;
    
    T *data_;
    size_t size_;

public:

    Sq_Matrix (const size_t size, const T init_val = T{}) : size_ {size}
    {
        data_ = new T[size * size]{init_val};
    }

    ~Sq_Matrix ()
    {
        delete[] data_;
    }
};

} // namespace Linear_Algebra

#endif // INCLUDE_MATRIX_HPP
