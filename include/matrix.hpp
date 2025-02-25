#ifndef INCLUDE_MATRIX_HPP
#define INCLUDE_MATRIX_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "container.hpp"
#include "floating_point_comparison.hpp"

namespace yLab
{

struct Undef_Operation : public std::runtime_error
{
    Undef_Operation(const char *message) : std::runtime_error{message} {};
};

struct Undef_Sum final : public Undef_Operation
{
    Undef_Sum() : Undef_Operation{"Sum of matrices of different sizes is not defined"} {};
};

struct Undef_Diff final : public Undef_Operation
{
    Undef_Diff() : Undef_Operation{"Difference of matrices of different sizes is not defined"} {};
};

struct Undef_Product final : public Undef_Operation
{
    Undef_Product() : Undef_Operation{"Product of matrices of different sizes is not defined"} {};
};

struct Undef_Det final : public Undef_Operation
{
    Undef_Det() : Undef_Operation{"Determinant is not defined for non-square matrices"} {};
};

struct Il_Il_Ctor_Fail final : public std::runtime_error
{
    Il_Il_Ctor_Fail()
        : std::runtime_error{"The number of elements in each row must be the same"} {}
};

template<typename T>
requires std::is_arithmetic_v<T>
class Matrix final : private Array<T>
{
    template<typename Ptr_T>
    struct Proxy_Row final
    {
        using Data_T = std::remove_pointer_t<Ptr_T>;

        Ptr_T row_;
        Data_T &operator[](std::size_t j) const { return row_[j]; }
    };

public:

    using typename Array<T>::value_type;
    using typename Array<T>::reference;
    using typename Array<T>::const_reference;
    using typename Array<T>::pointer;
    using typename Array<T>::const_pointer;
    using typename Array<T>::size_type;
    using typename Array<T>::iterator;
    using typename Array<T>::const_iterator;
    using typename Array<T>::reverse_iterator;
    using typename Array<T>::const_reverse_iterator;

    using Array<T>::begin;
    using Array<T>::end;
    using Array<T>::cbegin;
    using Array<T>::cend;
    using Array<T>::rbegin;
    using Array<T>::rend;
    using Array<T>::crbegin;
    using Array<T>::crend;
    using Array<T>::data;
    using Array<T>::size;

    // Constructors
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix(size_type n_rows, size_type n_cols, value_type value = value_type{})
        : Array<T>(n_rows * n_cols, value), n_rows_{n_rows}, n_cols_{n_cols} {}

    Matrix(std::initializer_list<std::initializer_list<value_type>> il_il)
        : Array<T>(il_il.size() * il_il.begin()->size()),
          n_rows_{il_il.size()}, n_cols_{il_il.begin()->size()}
    {
        for (size_type row_i = 0; const auto &internal_list : il_il)
        {
            if (internal_list.size() != n_cols_)
                throw Il_Il_Ctor_Fail{};

            std::copy(internal_list.begin(), internal_list.end(), begin() + row_i * n_cols_);
            ++row_i;
        }
    }

    template<std::input_iterator Iter>
    Matrix(size_type n_rows, size_type n_cols, Iter begin, Iter end)
        : Array<T>(n_rows * n_cols), n_rows_{n_rows}, n_cols_{n_cols}
    {
        size_type i = 0;
        for (auto iter = begin; iter != end && i != size(); ++iter, ++i)
            data()[i] = *iter;

        for (; i != size(); ++i)
            data()[i] = value_type{};
    }

    static Matrix identity_matrix(size_type n_rows, size_type n_cols)
    {
        Matrix res{n_rows, n_cols};
        const size_type min_size = std::min(n_rows, n_cols);
        for (size_type diag_i = 0; diag_i != min_size; ++diag_i)
            res[diag_i][diag_i] = value_type{1};
        return res;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Fields access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    size_type n_cols() const noexcept { return n_cols_; }
    size_type n_rows() const noexcept { return n_rows_; }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Elements access
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Proxy_Row<const value_type *> operator[](size_type row_i) const
    {
        return Proxy_Row{data() + row_i * n_cols_};
    }

    Proxy_Row<value_type *> operator[](size_type row_i)
    {
        return Proxy_Row{data() + row_i * n_cols_};
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Some convenient methods
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    static bool are_congruent(const Matrix &lhs, const Matrix &rhs) noexcept
    {
        return lhs.n_rows_ == rhs.n_rows_ && lhs.n_cols_ == rhs.n_cols_;
    }

    bool is_square() const noexcept { return n_rows_ == n_cols_; }

    Matrix &transpose() &
    {
        if (is_square())
        {
            for (size_type i = 0; i != n_rows_; ++i)
                for (size_type j = i + 1; j != n_cols_; ++j)
                    std::swap((*this)[i][j], (*this)[j][i]);
        }
        else
        {
            Matrix transposed{n_cols_, n_rows_};

            for (size_type i = 0; i != n_rows_; ++i)
                for (size_type j = 0; j != n_cols_; ++j)
                    transposed[j][i] = (*this)[i][j];

            std::swap(*this, transposed);
        }

        return *this;
    }

    value_type determinant() const
    {
        if (!is_square())
            throw Undef_Det{};
        return Matrix{*this}.det_algorithm();
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // Arithmetic operators
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Matrix &operator+=(const Matrix &rhs)
    {
        if (!are_congruent(*this, rhs))
            throw Undef_Sum{};

        std::transform(begin(), end(), rhs.begin(), begin(), std::plus<value_type>{});
        return *this;
    }

    Matrix &operator-=(const Matrix &rhs)
    {
        if (!are_congruent(*this, rhs))
            throw Undef_Diff{};

        std::transform(begin(), end(), rhs.begin(), begin(), std::minus<value_type>{});
        return *this;
    }

    Matrix &operator*=(const value_type &value)
    {
        std::transform(begin(), end(), begin(),
                       [&value](const value_type &elem){ return elem * value; });
        return *this;
    }

    Matrix &operator/=(const value_type &value)
    {
        std::transform(begin(), end(), begin(),
                       [&value](const value_type &elem){ return elem / value; });
        return *this;
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

private:

    // Gauss algorithm
    value_type det_algorithm()
    requires std::is_floating_point_v<value_type>
    {
        auto exchanges = 0;

        size_type row_i = 0;
        size_type col_i = 0;

        while (row_i < n_rows_ && col_i < n_cols_)
        {
            const auto [pivot_pos, pivot] = find_pivot(row_i, col_i);

            if (pivot == value_type{})
                return value_type{};
            else
            {
                if (row_i != pivot_pos)
                {
                    swap_rows(row_i, pivot_pos);
                    ++exchanges;
                }

                for (size_type i = row_i + 1; i != n_cols_; ++i)
                {
                    const value_type coeff = (*this)[i][col_i] / (*this)[row_i][col_i];
                    (*this)[i][col_i] = value_type{};

                    for (size_type j = col_i + 1; j != n_rows_; ++j)
                        (*this)[i][j] -= (*this)[row_i][j] * coeff;
                }

                ++row_i;
                ++col_i;
            }
        }

        value_type determinant = (*this)[0][0];
        for (size_type diag_i = 1; diag_i != n_cols_; ++diag_i)
            determinant *= (*this)[diag_i][diag_i];

        if (yLab::cmp::are_equal(determinant, value_type{}))
            return value_type{};
        return (exchanges % 2) ? -determinant : determinant;
    }

    // Bareiss algorithm
    value_type det_algorithm()
    requires std::is_integral_v<value_type>
    {
        auto exchanges = 0;

        value_type init_val{1};

        for (size_type row_i = 0; row_i != n_rows_ - 1; ++row_i)
        {
            const auto [pivot_pos, pivot] = find_pivot(row_i, row_i);

            if (pivot == value_type{})
                return value_type{};
            else
            {
                if (row_i != pivot_pos)
                {
                    swap_rows(row_i, pivot_pos);
                    ++exchanges;
                }

                const value_type value_1 = (*this)[row_i][row_i];

                for (size_type i = row_i + 1; i != n_cols_; ++i)
                {
                    const auto value_2 = std::exchange((*this)[i][row_i], value_type{});

                    for (size_type j = row_i + 1; j != n_rows_; ++j)
                        (*this)[i][j] = ((*this)[i][j] * value_1 -
                                         (*this)[row_i][j] * value_2) / init_val;
                }

                init_val = value_1;
            }
        }

        const value_type determinant = (*this)[n_rows_ - 1][n_cols_ - 1];

        return (exchanges % 2) ? -determinant : determinant;
    }

    std::pair<size_type, value_type> find_pivot(size_type begin_row, size_type col) const
    {
        std::pair<size_type, value_type> pivot{begin_row, (*this)[begin_row][col]};

        for (size_type row_i = begin_row + 1; row_i != n_rows_; ++row_i)
        {
            const value_type elem = (*this)[row_i][col];
            if (std::abs(pivot.second) < std::abs(elem))
            {
                pivot.first  = row_i;
                pivot.second = elem;
            }
        }

        return pivot;
    }

    void swap_rows(size_type row_1, size_type row_2)
    {
        std::swap_ranges(begin() + row_1 * n_cols_, begin() + (row_1 + 1) * n_cols_,
                         begin() + row_2 * n_cols_);
    }

    size_type n_rows_;
    size_type n_cols_;
};

template<typename T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    if (&lhs == &rhs)
        return true;
    else if (!Matrix<T>::are_congruent(lhs, rhs))
        return false;
    else
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T>
Matrix<T> operator+(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    auto sum = lhs;
    return sum += rhs;
}

template<typename T>
Matrix<T> operator-(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    auto diff = lhs;
    return diff -= rhs;
}

template<typename T>
Matrix<T> operator*(const Matrix<T> &lhs, const T &value)
{
    auto mult = lhs;
    return mult *= value;
}

template<typename T>
Matrix<T> operator*(const T &value, const Matrix<T> &lhs) { return lhs * value; }

template<typename T>
Matrix<T> operator/(const Matrix<T> &lhs, const T &value)
{
    auto div = lhs;
    return div /= value;
}

template<typename T>
Matrix<T> product(const Matrix<T> &lhs, const Matrix<T> &rhs)
{
    if (lhs.n_cols() != rhs.n_rows())
        throw Undef_Product{};

    Matrix<T> product{lhs.n_rows(), rhs.n_cols()};

    for (std::size_t i = 0; i != lhs.n_rows(); ++i)
        for (std::size_t j = 0; j != rhs.n_cols(); ++j)
            for (std::size_t k = 0; k != lhs.n_cols(); ++k)
                product[i][j] += lhs[i][k] * rhs[k][j];

    return product;
}

template<typename T>
void dump(std::ostream &os, const Matrix<T> &matrix)
{
    os.setf(std::ios::left);

    for (std::size_t i = 1; auto &elem : matrix)
    {
        if (i++ % matrix.n_cols())
            os << std::setw(5) << elem << ' ';
        else
            os << elem << '\n';
    }
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix)
{
    dump(os, matrix);
    return os;
}

} // namespace yLab

#endif // INCLUDE_MATRIX_HPP
