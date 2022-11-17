#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include <stdexcept>
#include <iostream>
#include <cstring>    // for std::memcmp

#include "container.hpp"
#include "floating_point_comparison.hpp"

namespace Linear_Algebra
{

struct Undef_Operation : public std::runtime_error
{
    Undef_Operation (const char *message)
                    : std::runtime_error {message}
    {};
};

struct Undef_Sum : public Undef_Operation
{
    Undef_Sum ()
              : Undef_Operation {"Sum of matrices of different size is not defined"}
    {};
};

struct Undef_Diff : public Undef_Operation
{
    Undef_Diff ()
               : Undef_Operation {"Difference of matrices of different size is not defined"}
    {};
};

struct Undef_Product : public Undef_Operation
{
    Undef_Product ()
                  : Undef_Operation {"Product of matrices of different size is not defined"}
    {};
};

struct Undef_Det : public Undef_Operation
{
    Undef_Det ()
              : Undef_Operation {"Determinant is not defined for non-square matrices"}
    {};
};

template <typename T> requires std::is_arithmetic<T>::value 
class Matrix final
{   
    Containers::Array<T> memory_;
    size_t n_rows_;
    size_t n_cols_;

    struct Proxy_Row
    {        
        T *row_;

        const T& operator[] (size_t j) const { return row_[j]; }
        T& operator[] (size_t j) { return row_[j]; }
    };

public:

    // Constructors
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix (const size_t n_rows, const size_t n_cols, const T init_val = T{})
              : memory_{n_rows * n_cols}, n_rows_{n_rows}, n_cols_{n_cols}
    {
        auto size = memory_.size();
        for (auto i = 0; i != size; ++i)
            memory_[i] = init_val;
    }

    template <std::input_iterator Iter>
    Matrix (const size_t n_rows, const size_t n_cols, Iter begin, Iter end)
              : memory_{n_rows * n_cols}, n_rows_{n_rows}, n_cols_{n_cols}
    {
        auto i = 0;
        const auto n_elems = memory_.size();
        for (auto iter = begin; iter != end && i != n_elems; ++iter, ++i)
            memory_[i] = *iter;

        while (i != n_elems)
            memory_[i++] = T{};
    }

    Matrix (const Matrix &rhs) = default;
    Matrix &operator= (const Matrix &rhs) = default;

    Matrix (Matrix &&rhs) = default;
    Matrix &operator= (Matrix &&rhs) = default;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Fields access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const T *data () const & { return memory_.data(); }
    T *data () & { return memory_.data(); }

    auto n_cols () const { return n_cols_; }
    auto n_rows () const { return n_rows_; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Elements access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const Proxy_Row operator[] (const size_t row_i) const
    {
        return Proxy_Row {data() + row_i * n_cols_};
    }

    Proxy_Row operator[] (const size_t row_i)
    {
        return Proxy_Row {data() + row_i * n_cols_};
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Some convenient methods
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix &transpose ()
    {
        // NOT FINISHED YET

        return *this;
    }

    T determinant () const
    {
        if (n_cols_ != n_rows_)
            throw Undef_Det {};
        
        auto copy = *this;
        return copy.algorithm ();
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // Arithmetic operators
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix &operator+= (const Matrix &rhs)
    {
        if (n_rows_ != rhs.n_rows_ || n_cols_ != rhs.n_cols_)
            throw Undef_Sum {};

        const auto size = memory_.size();
        for (auto i = 0; i != size; ++i)
            memory_[i] += rhs.memory_[i];
    }

    Matrix &operator-= (const Matrix &rhs)
    {
        if (n_rows_ != rhs.n_rows_ || n_cols_ != rhs.n_cols_)
            throw Undef_Diff {};

        const auto size = memory_.size();
        for (auto i = 0; i != size; ++i)
            memory_[i] -= rhs.memory_[i];
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    static Matrix identity_matrix (const size_t n_rows, const size_t n_cols)
    {
        Matrix<T> res {n_rows, n_cols};
        auto min_size = std::min (n_rows, n_cols);
        for (auto diag_i = 0; diag_i < min_size; diag_i++)
            res.memory_[diag_i * (n_cols + 1)] = T{1};

        return res;
    }

    void dump (std::ostream &os = std::cout) const
    {
        for (auto i = 0; i != n_cols_; ++i)
        {
            for (auto j = 0; j != n_rows_; ++j)
                os << memory_[i * n_cols_ + j] << " ";
            os << std::endl;
        }
    }

private:

    // Gauss algorithm
    T algorithm () requires std::is_floating_point<T>::value
    {
        auto exchanges = 0;
        
        size_t row_i {};
        size_t col_i {};

        while (row_i < n_rows_ && col_i < n_cols_)
        {
            const auto [pivot_pos, pivot] = find_pivot (row_i, col_i);

            if (pivot == T{})
                return pivot;
            else
            {
                if (row_i != pivot_pos)
                {
                    swap_rows (row_i, pivot_pos);
                    exchanges++;
                }
                
                for (auto i = row_i + 1; i != n_cols_; ++i)
                {
                    auto coeff = memory_[i * n_cols_ + col_i] / memory_[row_i * n_cols_ + col_i];
                    memory_[i * n_cols_ + col_i] = T{};

                    for (auto j = col_i + 1; j != n_rows_; ++j)
                        memory_[i * n_cols_ + j] -= memory_[row_i * n_cols_ + j] * coeff;
                }

                row_i++;
                col_i++;
            }
        }

        T determinant = memory_[0];
        for (auto diag_i = 1; diag_i != n_cols_; ++diag_i)
            determinant *= memory_[diag_i * (n_cols_ + 1)];

        if (cmp::are_equal (determinant, 0.0))
            determinant = 0.0;

        return (exchanges % 2) ? -determinant : determinant;
    }

    // Bareiss algorithm
    T algorithm () requires std::is_integral<T>::value
    {
        auto exchanges = 0;

        T init_val {1};

        for (auto row_i = 0; row_i != n_rows_ - 1; ++row_i)
        {
            auto [pivot_pos, pivot] = find_pivot (row_i, row_i);

            if (pivot == T{})
                return pivot;
            else
            {
                if (row_i != pivot_pos)
                {
                    swap_rows (row_i, pivot_pos);
                    exchanges++;
                }

                const auto value_1 = memory_[row_i * (n_cols_ + 1)];

                for (auto i = row_i + 1; i != n_cols_; ++i)
                {
                    const auto value_2 = memory_[i * n_cols_ + row_i];   
                    memory_[i * n_cols_ + row_i] = T{};

                    for (auto j = row_i + 1; j != n_rows_; j++)
                        memory_[i * n_cols_ + j] = (memory_[i * n_cols_ + j]     * value_1 - 
                                                    memory_[row_i * n_cols_ + j] * value_2) / init_val;
                }

                init_val = value_1;
            }
        }

        auto determinant = memory_[n_cols_ * n_cols_ - 1];

        return (exchanges % 2) ? -determinant : determinant;
    }

    std::pair<size_t, T> find_pivot (const size_t begin_row, const size_t col) const
    {
        std::pair<size_t, T> pivot {begin_row, memory_[begin_row * n_cols_ + col]};

        for (auto row_i = begin_row + 1; row_i != n_rows_; ++row_i)
        {
            auto elem = memory_[row_i * n_cols_ + col];
            if (std::abs (pivot.second) < std::abs (elem))
            {
                pivot.first  = row_i;
                pivot.second = elem;
            }
        }
    
        return pivot;
    }

    void swap_rows (const size_t row_1, const size_t row_2)
    {
        for (auto col_i = 0; col_i != n_cols_; ++col_i)
            std::swap (memory_[row_1 * n_cols_ + col_i], memory_[row_2 * n_cols_ + col_i]);
    }
};

template <typename T>
static bool operator== (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    if (&lhs == &rhs)
        return true;
    else if (lhs.n_rows() != rhs.n_rows() || lhs.n_cols() != rhs.n_cols())
        return false;
    else
        return std::memcmp (lhs.data(), rhs.data(), lhs.n_rows() * lhs.n_cols() * sizeof (T)) == 0;
}

template <typename T>
bool operator!= (const Matrix<T> &lhs, const Matrix<T> &rhs) { return !(lhs == rhs); }

template <typename T>
std::ostream &operator<< (std::ostream &os, const Matrix<T> &matrix)
{
    matrix.dump(os);
    return os;
}

template <typename T>
Matrix<T> operator+ (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    auto sum = lhs;
    return sum += rhs;
}

template <typename T>
Matrix<T> operator- (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    auto diff = lhs;
    return diff -= rhs;
}

template <typename T>
Matrix<T> product (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    if (lhs.n_cols_ != rhs.n_rows_)
        throw Undef_Product {};

    Matrix<T> product {lhs.n_rows_, rhs.n_cols_};
    auto dim = lhs.n_cols_;

    for (auto i = 0; i != lhs.n_rows_; ++i)
    {
        for (auto j = 0; j != rhs.n_cols_; ++j)
        {
            for (auto k = 0; k != dim; ++k)
                product.memory_[i * product.n_cols_ + j] += lhs.memory_[i * dim + k] * 
                                                            rhs.memory_[k * rhs.n_cols_ + j];
        }
    }
}

} // namespace Linear_Algebra

#endif // INCLUDE_MATRIX_HPP
