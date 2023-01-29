#ifndef INCLUDE_RANDOM_MATRIX
#define INCLUDE_RANDOM_MATRIX

#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace yLab
{

template <typename T> requires std::is_integral_v<T>
struct Random_Matrix final
{
    size_t size_;
    std::vector<std::vector<T>> memory_;

    Random_Matrix (const size_t size, const T det) : size_{size}, memory_(size)
    {
        for (auto i = 0; i != size_; ++i)
        {
            for (auto j = 0; j != size_; ++j)
            {
                if (i == j)
                    memory_[i].push_back ((i == 0) ? det : T{1});
                else
                    memory_[i].push_back (T{});
            }
        }

        std::random_device rd;
        std::mt19937_64 gen (rd());

        std::uniform_int_distribution<size_t> rows (0, size - 1);
        std::uniform_int_distribution<T> mult (-size / 2, size / 2);

        auto n_shuffles = size * 2;
        for (auto shuffle_i = 0; shuffle_i != n_shuffles; ++shuffle_i)
            strings_transformation (rows (gen), rows (gen), mult (gen));

        for (auto shuffle_i = 0; shuffle_i != n_shuffles; ++shuffle_i)
            columns_transformation (rows (gen), rows (gen), mult (gen));
    }

    void strings_transformation (size_t row_1, size_t row_2, const T mult)
    {
        if (row_1 == row_2)
            return;

        for (auto j = 0; j != size_; ++j)
            memory_[row_1][j] += memory_[row_2][j] * mult;
    }

    void columns_transformation (size_t col_1, size_t col_2, const T mult)
    {
        if (col_1 == col_2)
            return;
        
        for (auto i = 0; i != size_; ++i)
            memory_[i][col_1] += memory_[i][col_2] * mult;
    }
};

template <typename T>
void dump (std::ostream &os, const Random_Matrix<T> &matrix)
{
    os.setf (std::ios::left);
    
    const auto size = matrix.size_;
    const auto data = matrix.memory_;

    for (auto i = 0, k = 1; i != size; ++i)
    {
        for (auto j = 0; j != size; ++j)
        {
            if (k++ % size == 0)
                os << data[i][j] << std::endl;
            else
                os << std::setw(5) << data[i][j] << " ";
        }
    }
}

template <typename T>
std::ostream &operator<< (std::ostream &os, const Random_Matrix<T> &matrix)
{
    dump(os, matrix);
    return os;
}

} // namespace yLab

#endif // INCLUDE_RANDOM_MATRIX
