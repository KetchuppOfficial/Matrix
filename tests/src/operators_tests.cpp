#include <gtest/gtest.h>
#include "matrix.hpp"

using namespace Matrix;

TEST (Operators, Elements_Access)
{
    constexpr auto size = 5;
    Sq_Matrix<int> m_1 {size};
    m_1[2][3] = 23;
    EXPECT_EQ (m_1.data()[2 * size + 3], 23);

    Sq_Matrix<int> m_2 {size, 2022};
    EXPECT_EQ (m_2[3][3], 2022);
}

TEST (Operators, Equality)
{
    std::vector<int> values = {1, 2, 3, 4};
    Sq_Matrix<int> m_1 {2, values.begin (), values.end ()};
    auto m_2 = m_1;
    auto m_3 = m_1;
    m_3[0][0] = 4;

    EXPECT_TRUE (m_1 == m_1);
    EXPECT_TRUE (m_1 == m_2);
    EXPECT_TRUE (m_1 != m_3);
}
