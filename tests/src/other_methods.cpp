#include <gtest/gtest.h>

#include "matrix.hpp"

TEST (Operators, Equality)
{
    yLab::Matrix<int> m_1 = {{1, 2},
                             {3, 4}};
    auto m_2 = m_1;
    auto m_3 = m_1;
    m_3[0][0] = 4;

    EXPECT_TRUE (m_1 == m_1);
    EXPECT_TRUE (m_1 == m_2);
    EXPECT_TRUE (m_1 != m_3);
}
