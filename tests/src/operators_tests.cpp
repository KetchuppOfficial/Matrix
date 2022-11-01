#include <gtest/gtest.h>
#include "matrix.hpp"

using namespace Linear_Algebra;

TEST (Operators, Elements_Access)
{
    constexpr auto n_rows = 5;
    constexpr auto n_cols = 7;
    Matrix<int> m_1 {n_rows, n_cols};
    m_1[2][3] = 23;
    EXPECT_EQ (m_1.data()[2 * n_cols + 3], 23);

    Matrix<int> m_2 {n_rows, n_cols, 2022};
    EXPECT_EQ (m_2[3][3], 2022);
}

TEST (Operators, Equality)
{
    std::vector<int> values = {1, 2, 3, 4};
    Matrix<int> m_1 {2, 2, values.begin (), values.end ()};
    auto m_2 = m_1;
    auto m_3 = m_1;
    m_3[0][0] = 4;

    EXPECT_TRUE (m_1 == m_1);
    EXPECT_TRUE (m_1 == m_2);
    EXPECT_TRUE (m_1 != m_3);
}
