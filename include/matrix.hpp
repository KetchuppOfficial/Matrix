#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include <algorithm> // for std::copy
#include <iostream>
#include <cstring>    // for std::memcmp

namespace Matrix
{

template <typename T>
concept arithmetic = std::is_arithmetic<T>::value;

template <arithmetic T>
class Sq_Matrix final
{
    using size_t = std::size_t;
    
    T *data_;
    size_t size_;

    struct Proxy_Row
    {        
        T *row_;

        const T& operator[] (size_t j) const { return row_[j]; }
        T& operator[] (size_t j) { return row_[j]; }
    };

public:

    // Constructors
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Sq_Matrix (const size_t size, const T init_val = T{})
              : data_{new T[size * size]}, size_{size}
    {
        const auto n_elems = size * size;
        for (auto i = 0; i != n_elems; ++i)
            data_[i] = init_val;
    }

    template <std::input_iterator Iter>
    Sq_Matrix (const size_t size, Iter begin, Iter end) : data_ {new T[size * size]}, size_{size}
    {
        auto i = 0;
        const auto n_elems = size * size;
        for (auto iter = begin; iter != end && i != n_elems; ++iter, ++i)
            data_[i] = *iter;

        while (i != n_elems)
            data_[i++] = T{};
    }

    Sq_Matrix (const Sq_Matrix &rhs) : data_{new T[rhs.size_ * rhs.size_]}, size_{rhs.size_}
    {
        std::copy (rhs.data_, rhs.data_ + size_ * size_, data_);
    }

    Sq_Matrix &operator= (const Sq_Matrix &rhs)
    {   
        if (this == &rhs)
            return *this;

        size_ = rhs.size_;
        delete[] data_;
        data_ = new T[size_ * size_];
        std::copy (rhs.data_, rhs.data_ + size_ * size_, data_);

        return *this;
    }

    Sq_Matrix (Sq_Matrix &&rhs) noexcept
              : data_{std::exchange (rhs.data_, nullptr)}, size_{std::move (rhs.size_)} {}

    Sq_Matrix &operator= (Sq_Matrix &&rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        delete[] rhs.data_;
        data_ = std::move (rhs.data_);
        rhs.data_ = nullptr;
        size_ = std::move (rhs.size_);

        return *this;
    }

    ~Sq_Matrix () { delete[] data_; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Fields access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const T *data () const noexcept { return data_; }
    T *data () noexcept { return data_; }

    auto size () const noexcept { return size_; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Elements access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const Proxy_Row operator[] (size_t row_i) const { return Proxy_Row {data_ + row_i * size_}; }
    Proxy_Row operator[] (size_t row_i) { return Proxy_Row {data_ + row_i * size_}; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Some convenient methods
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Sq_Matrix &transpose () &
    {
        for (auto i = 0; i != size_; ++i)
            for (auto j = 0; j != size_; ++j)
                std::swap (*this[i][j], *this[j][i]);
    }

    T determinant () const
    {
        auto copy = *this;

        //return copy.gauss_algorithm ();

        if (std::is_integral<T>::value)
            return copy.bareiss_algorithm ();
        else
            return copy.gauss_algorithm ();
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    static Sq_Matrix identity_matrix (const size_t size)
    {
        Sq_Matrix<T> res {size};
        for (auto diag_elem = 0; diag_elem < size; diag_elem++)
            res.data_[diag_elem * (size + 1)] = static_cast<T>(1);

        return res;
    }

    void dump (std::ostream &os) const
    {
        for (auto i = 0; i != size_; ++i)
        {
            for (auto j = 0; j != size_; ++j)
                os << data_[i * size_ + j] << " ";
            os << std::endl;
        }
    }

private:

    T gauss_algorithm ()
    {     
        auto exchanges = 0;

        std::cout << *this << std::endl;

        for (auto step = 0; step != size_ - 1; ++step)
        {
            auto row_i = step;

            for (; row_i != size_; ++row_i)
                if (data_[row_i * size_ + step] != T{})
                    break;

            if (row_i == size_)
                return T{};
            else if (row_i != step)
            {
                swap_rows (step, row_i);
                exchanges++;
            }

            std::cout << *this << std::endl;

            auto leader = data_[step * (size_ + 1)];
            for (auto row_i = step + 1; row_i != size_; ++row_i)
            {
                auto coeff = -data_[row_i * size_ + step] / leader;
                
                data_[row_i * size_ + step] = T{};
                for (auto column_i = step + 1; column_i != size_; ++column_i)
                    data_[row_i * size_ + column_i] += coeff * data_[step * size_ + column_i];
            }

            std::cout << *this << std::endl;
        }

        T determinant = data_[0];
        for (auto i = 1; i != size_; ++i)
            determinant *= data_[i * (size_ + 1)];

        return (exchanges % 2) ? -determinant : determinant;
    }

    void swap_rows (const size_t row_1, const size_t row_2)
    {
        for (auto column_i = 0; column_i != size_; ++column_i)
            std::swap (data_[row_1 * size_ + column_i], data_[row_2 * size_ + column_i]);
    }

    T bareiss_algorithm ()
    {
        T init_val {1};
        auto exchanges = 0;

        for (auto l_1 = 0; l_1 != size_ - 1; ++l_1)
        {
            auto max_n = l_1;
            auto max_value = abs (data_[l_1 * (size_ + 1)]);

            for (auto l_2 = l_1 + 1; l_2 != size_; ++l_2)
            {
                const auto value = abs (data_[l_2 * size_ + l_2]);
                if (value > max_value)
                {
                    max_n = l_2;
                    max_value = value;
                }
            }

            if (max_n > l_1)
            {
                swap_rows (l_1, max_n);
                exchanges++;
            }
            else
            {
                if (max_value == T{})
                    return max_value;
            }

            const auto value_1 = data_[l_1 * (size_ + 1)];

            for (auto l_2 = l_1 + 1; l_2 != size_; ++l_2)
            {
                const auto value_2 = data_[l_2 * size_ + l_1];
                
                data_[l_2 * size_ + l_1] = T{};
                for (auto c = l_1 + 1; c != size_; c++)
                    data_[l_2 * size_ + c] = (data_[l_2 * size_ + c] * value_1 - data_[l_1 * size_ + c] * value_2) / init_val;
            }

            init_val = value_1;
        }

        auto determinant = data_[size_ * size_ - 1];

        return (exchanges % 2) ? -determinant : determinant;
    } 
};

template <typename T>
static bool operator== (const Sq_Matrix<T> &lhs, const Sq_Matrix<T> &rhs)
{
    if (&lhs == &rhs)
        return true;
    else if (lhs.size() != rhs.size())
        return false;
    else
        return std::memcmp (lhs.data(), rhs.data(), lhs.size() * lhs.size() * sizeof (T)) == 0;
}

template <typename T>
bool operator!= (const Sq_Matrix<T> &lhs, const Sq_Matrix<T> &rhs) { return !(lhs == rhs); }

template <typename T>
std::ostream &operator<< (std::ostream &os, const Sq_Matrix<T> &matrix)
{
    matrix.dump(os);
    return os;
}

} // namespace Matrix

#endif // INCLUDE_MATRIX_HPP
