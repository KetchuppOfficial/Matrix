#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include <algorithm> // for std::copy
#include <iostream>
#include <cstring>    // for std::memcmp

namespace Linear_Algebra
{

namespace cmp
{

constexpr double epsilon = 1e-6;

inline bool are_equal (const double first, const double second,
                       const double zero_diff = epsilon,
                       const double rel_diff  = epsilon)
{
    auto diff = std::abs (first - second);

    if (diff < zero_diff)
        return true;
    else
        return (diff < std::max (std::abs (first), std::abs (second)) * rel_diff);
}

} // namespace cmp

template <typename T> requires std::is_arithmetic<T>::value 
class Matrix final
{
    using size_t = std::size_t;
    
    T *data_;
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
              : data_{new T[n_rows * n_cols]}, n_rows_{n_rows}, n_cols_{n_cols}
    {
        const auto n_elems = n_rows * n_cols;
        for (auto i = 0; i != n_elems; ++i)
            data_[i] = init_val;
    }

    template <std::input_iterator Iter>
    Matrix (const size_t n_rows, const size_t n_cols, Iter begin, Iter end)
              : data_ {new T[n_rows * n_cols]}, n_rows_{n_rows}, n_cols_{n_cols}
    {
        auto i = 0;
        const auto n_elems = n_rows * n_cols;
        for (auto iter = begin; iter != end && i != n_elems; ++iter, ++i)
            data_[i] = *iter;

        while (i != n_elems)
            data_[i++] = T{};
    }

    Matrix (const Matrix &rhs) : data_{new T[rhs.n_rows_ * rhs.n_cols_]}, 
                                       n_rows_{rhs.n_rows_}, n_cols_{rhs.n_cols_}
    {
        std::copy (rhs.data_, rhs.data_ + n_rows_ * n_cols_, data_);
    }

    Matrix &operator= (const Matrix &rhs)
    {   
        if (this == &rhs)
            return *this;

        n_rows_ = rhs.n_rows_;
        n_cols_ = rhs.n_cols_;
        delete[] data_;
        data_ = new T[n_rows_ * n_cols_];
        std::copy (rhs.data_, rhs.data_ + n_rows_ * n_cols_, data_);

        return *this;
    }

    Matrix (Matrix &&rhs) noexcept
              : data_{std::exchange (rhs.data_, nullptr)}, 
                n_rows_{std::move (rhs.n_rows_)},
                n_cols_{std::move (rhs.n_cols_)}
    {}

    Matrix &operator= (Matrix &&rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        delete[] data_;
        data_ = std::move (rhs.data_);
        rhs.data_ = nullptr;

        n_rows_ = std::move (rhs.n_rows_);
        n_cols_ = std::move (rhs.n_cols_);

        return *this;
    }

    ~Matrix () { delete[] data_; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Fields access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const T *data () const noexcept { return data_; }
    T *data () noexcept { return data_; }

    auto n_cols () const noexcept { return n_cols_; }
    auto n_rows () const noexcept { return n_rows_; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Elements access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    const Proxy_Row operator[] (const size_t row_i) const
    {
        return Proxy_Row {data_ + row_i * n_cols_};
    }

    Proxy_Row operator[] (const size_t row_i)
    {
        return Proxy_Row {data_ + row_i * n_cols_};
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Some convenient methods
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix &transpose ()
    {
        std::swap (n_rows_, n_cols_);

        return *this;
    }

    T determinant () const
    {
        if (n_cols_ != n_rows_)
            throw std::runtime_error ("Determinant is only defined for square matrices");
        
        auto copy = *this;
        return copy.algorithm ();
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    static Matrix identity_matrix (const size_t n_rows, const size_t n_cols)
    {
        Matrix<T> res {n_rows, n_cols};
        auto min_size = std::min (n_rows, n_cols);
        for (auto diag_i = 0; diag_i < min_size; diag_i++)
            res.data_[diag_i * (n_cols + 1)] = T{1};

        return res;
    }

    void dump (std::ostream &os) const
    {
        for (auto i = 0; i != n_cols_; ++i)
        {
            for (auto j = 0; j != n_rows_; ++j)
                os << data_[i * n_cols_ + j] << " ";
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
                    auto coeff = data_[i * n_cols_ + col_i] / data_[row_i * n_cols_ + col_i];
                    data_[i * n_cols_ + col_i] = T{};

                    for (auto j = col_i + 1; j != n_rows_; ++j)
                        data_[i * n_cols_ + j] -= data_[row_i * n_cols_ + j] * coeff;
                }

                row_i++;
                col_i++;
            }
        }

        T determinant = data_[0];
        for (auto diag_i = 1; diag_i != n_cols_; ++diag_i)
            determinant *= data_[diag_i * (n_cols_ + 1)];

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

                const auto value_1 = data_[row_i * (n_cols_ + 1)];

                for (auto i = row_i + 1; i != n_cols_; ++i)
                {
                    const auto value_2 = data_[i * n_cols_ + row_i];   
                    data_[i * n_cols_ + row_i] = T{};

                    for (auto j = row_i + 1; j != n_rows_; j++)
                        data_[i * n_cols_ + j] = (data_[i * n_cols_ + j]     * value_1 - 
                                                  data_[row_i * n_cols_ + j] * value_2) / init_val;
                }

                init_val = value_1;
            }
        }

        auto determinant = data_[n_cols_ * n_cols_ - 1];

        return (exchanges % 2) ? -determinant : determinant;
    }

    std::pair<size_t, T> find_pivot (const size_t begin_row, const size_t col) const
    {
        std::pair<size_t, T> pivot {begin_row, data_[begin_row * n_cols_ + col]};

        for (auto row_i = begin_row + 1; row_i != n_rows_; ++row_i)
        {
            auto elem = data_[row_i * n_cols_ + col];
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
            std::swap (data_[row_1 * n_cols_ + col_i], data_[row_2 * n_cols_ + col_i]);
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

} // namespace Linear_Algebra

#endif // INCLUDE_MATRIX_HPP
