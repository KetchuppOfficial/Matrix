#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include <stdexcept>
#include <initializer_list>
#include <iostream>
#include <iomanip>
#include <algorithm>

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

struct Il_Il_Ctor_Fail : public std::runtime_error
{
    Il_Il_Ctor_Fail ()
                    : std::runtime_error {"The quantity of elements in each row must be the same"}
    {}
};

template <typename T> requires std::is_arithmetic<T>::value 
class Matrix final
{   
    Containers::Array<T> memory_;
    size_t n_rows_;
    size_t n_cols_;

    struct const_Proxy_Row
    {        
        const T *row_;
        const T& operator[] (size_t j) const { return row_[j]; }
    };

    struct Proxy_Row
    {
        T *row_;
        T& operator[] (size_t j) { return row_[j]; }
    };

public:

    // Constructors
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix (const size_t n_rows, const size_t n_cols, const T init_val = T{})
              : memory_{n_rows * n_cols}, n_rows_{n_rows}, n_cols_{n_cols}
    {
        std::fill (begin(), end(), init_val);
    }

    Matrix (std::initializer_list<std::initializer_list<T>> il)
           : memory_{il.size() * il.begin()->size()}, 
             n_rows_{il.size()}, 
             n_cols_{il.begin()->size()}
    {
        auto i = 0;
        for (auto out_iter = il.begin(), o_end = il.end(); out_iter != o_end; ++out_iter)
        {
            if (out_iter->size() != n_cols_)
                throw Il_Il_Ctor_Fail{};

            for (auto in_iter = out_iter->begin(), i_end = out_iter->end(); in_iter != i_end; ++in_iter)
                memory_[i++] = *in_iter;
        }
    }

    template <std::input_iterator Iter>
    Matrix (const size_t n_rows, const size_t n_cols, Iter begin, Iter end)
              : memory_{n_rows * n_cols}, n_rows_{n_rows}, n_cols_{n_cols}
    {
        auto i = 0;
        for (auto iter = begin; iter != end && i != size(); ++iter, ++i)
            memory_[i] = *iter;

        for (; i != size(); ++i)
            memory_[i] = T{};
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

    auto size () const { return memory_.size(); }

    auto n_cols () const { return n_cols_; }
    auto n_rows () const { return n_rows_; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Elements access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const const_Proxy_Row operator[] (const size_t row_i) const
    {
        return const_Proxy_Row {data() + row_i * n_cols_};
    }

    Proxy_Row operator[] (const size_t row_i)
    {
        return Proxy_Row {data() + row_i * n_cols_};
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Iterators
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    class iterator final
    {
        using iterator_category = typename std::input_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        T *ptr_;

    public:

        iterator (T *ptr) : ptr_{ptr} {}

        iterator (const iterator &iter) = default;
        iterator &operator= (const iterator &iter) = default;

        iterator (iterator &&iter) = default;
        iterator &operator= (iterator &&iter) = default;

        T &operator* () { return *ptr_; }

        iterator &operator++ () { return (ptr_++, *this); }

        iterator operator++ (int)
        {
            auto iter_copy = *this;
            ptr_++;
            return iter_copy;
        }

        iterator &operator-- () { return (ptr_--, *this); }

        iterator operator-- (int)
        {
            auto iter_copy = *this;
            ptr_--;
            return iter_copy;
        }

        bool operator<=> (const iterator &iter) const = default; 
    };

    class const_iterator final
    {
        using iterator_category = typename std::input_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        const T *ptr_;

    public:

        const_iterator (const T *ptr) : ptr_{ptr} {}

        const_iterator (const const_iterator &iter) = default;
        const_iterator &operator= (const const_iterator &iter) = default;

        const_iterator (const_iterator &&iter) = default;
        const_iterator &operator= (const_iterator &&iter) = default;

        const T &operator* () const { return *ptr_; }

        const_iterator &operator++ () { return (ptr_++, *this); }

        const_iterator operator++ (int)
        {
            auto iter_copy = *this;
            ptr_++;
            return iter_copy;
        }

        const_iterator &operator-- () { return (ptr_--, *this); }

        const_iterator operator-- (int)
        {
            auto iter_copy = *this;
            ptr_--;
            return iter_copy;
        }

        bool operator<=> (const const_iterator &iter) const = default; 
    };

    auto begin () { return iterator{data()}; }
    auto begin () const { return const_iterator{data()}; }
    auto cbegin () const { return const_iterator {data()}; }

    auto end () { return iterator{data() + size()}; }
    auto end () const { return const_iterator{data() + size()}; }
    auto cend () const { return const_iterator{data() + size()}; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Some convenient methods
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix &transpose () &
    {
        Matrix<T> transposed {n_cols_, n_rows_};
        for (auto i = 0; i != n_rows_; ++i)
            for (auto j = 0; j != n_cols_; ++j)
                transposed[j][i] = (*this)[i][j];

        std::swap (*this, transposed);
        return *this;
    }

    T determinant () const
    {
        if (n_cols_ != n_rows_)
            throw Undef_Det {};
        
        auto copy = *this;
        auto det  = copy.det_algorithm ();
        
        return det;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // Arithmetic operators
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix &operator+= (const Matrix &rhs)
    {
        if (n_rows_ != rhs.n_rows_ || n_cols_ != rhs.n_cols_)
            throw Undef_Sum {};

        for (auto i = 0; i != size(); ++i)
            memory_[i] += rhs.memory_[i];
    }

    Matrix &operator-= (const Matrix &rhs)
    {
        if (n_rows_ != rhs.n_rows_ || n_cols_ != rhs.n_cols_)
            throw Undef_Diff {};

        for (auto i = 0; i != size(); ++i)
            memory_[i] -= rhs.memory_[i];
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    static Matrix identity_matrix (const size_t n_rows, const size_t n_cols)
    {
        Matrix<T> res {n_rows, n_cols};
        auto min_size = std::min (n_rows, n_cols);
        for (auto diag_i = 0; diag_i < min_size; diag_i++)
            res[diag_i][diag_i] = T{1};

        return res;
    }

private:

    // Gauss algorithm
    T det_algorithm () requires std::is_floating_point<T>::value
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
                    auto coeff = (*this)[i][col_i] / (*this)[row_i][col_i];
                    (*this)[i][col_i] = T{};

                    for (auto j = col_i + 1; j != n_rows_; ++j)
                        (*this)[i][j] -= (*this)[row_i][j] * coeff;
                }

                row_i++;
                col_i++;
            }
        }

        T determinant = (*this)[0][0];
        for (auto diag_i = 1; diag_i != n_cols_; ++diag_i)
            determinant *= (*this)[diag_i][diag_i];

        if (cmp::are_equal (determinant, 0.0))
            determinant = 0.0;

        return (exchanges % 2) ? -determinant : determinant;
    }

    // Bareiss algorithm
    T det_algorithm () requires std::is_integral<T>::value
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

                const auto value_1 = (*this)[row_i][row_i];

                for (auto i = row_i + 1; i != n_cols_; ++i)
                {
                    const auto value_2 = std::exchange ((*this)[i][row_i], T{});

                    for (auto j = row_i + 1; j != n_rows_; j++)
                        (*this)[i][j] = ((*this)[i][j] * value_1 - 
                                         (*this)[row_i][j] * value_2) / init_val;
                }

                init_val = value_1;
            }
        }

        auto determinant = (*this)[n_rows_ - 1][n_cols_ - 1];

        return (exchanges % 2) ? -determinant : determinant;
    }

    std::pair<size_t, T> find_pivot (const size_t begin_row, const size_t col) const
    {
        std::pair<size_t, T> pivot {begin_row, (*this)[begin_row][col]};

        for (auto row_i = begin_row + 1; row_i != n_rows_; ++row_i)
        {
            auto elem = (*this)[row_i][col];
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
        std::swap_ranges (&(*this)[row_1][0], &(*this)[row_1][n_cols_], &(*this)[row_2][0]);
        // This looks ugly
    }
};

template <typename T>
bool operator== (const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    if (&lhs == &rhs)
        return true;
    else if (lhs.n_rows() != rhs.n_rows() || lhs.n_cols() != rhs.n_cols())
        return false;
    else
        return std::equal (lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
bool operator!= (const Matrix<T> &lhs, const Matrix<T> &rhs) { return !(lhs == rhs); }

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
    if (lhs.n_cols() != rhs.n_rows())
        throw Undef_Product {};

    Matrix<T> product {lhs.n_rows(), rhs.n_cols()};
    auto dim = lhs.n_cols();

    for (auto i = 0; i != lhs.n_rows(); ++i)
    {
        for (auto j = 0; j != rhs.n_cols(); ++j)
        {
            for (auto k = 0; k != dim; ++k)
                product[i][j] += lhs[i][k] * rhs[k][j];
        }
    }

    return product;
}

template <typename T>
void dump (std::ostream &os, const Matrix<T> &matrix)
{   
    os.setf (std::ios::left);
    
    const auto n_cols = matrix.n_cols();
    for (auto i = 1; auto &elem : matrix)
    {
        if (i++ % n_cols == 0)
            os << elem << std::endl;
        else
            os << std::setw(5) << elem << " ";
    }
}

template <typename T>
std::ostream &operator<< (std::ostream &os, const Matrix<T> &matrix)
{
    dump(os, matrix);
    return os;
}

} // namespace Linear_Algebra

#endif // INCLUDE_MATRIX_HPP
