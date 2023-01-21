#include <gtest/gtest.h>
#include "matrix.hpp"

TEST (Elements_Access, Square_Brackets)
{
    constexpr auto n_rows = 5;
    constexpr auto n_cols = 7;
    constexpr auto value = 213;

    yLab::Matrix<int> m_1 {n_rows, n_cols};
    m_1[2][3] = value;
    EXPECT_EQ (m_1.data()[2 * n_cols + 3], value);

    yLab::Matrix<int> m_2 {n_rows, n_cols, value};
    for (auto i = 0; i != n_rows; ++i)
        for (auto j = 0; j != n_cols; ++j)
            EXPECT_EQ (m_2[i][j], value);
}
