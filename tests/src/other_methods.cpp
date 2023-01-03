#include <gtest/gtest.h>

#include "matrix.hpp"

TEST (Other_Methods, Equality)
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

TEST (Other_Methods, Are_Congruent)
{
    yLab::Matrix<int> m_1 {2, 2};

    yLab::Matrix<int> m_2 {2, 3};
    EXPECT_FALSE (yLab::Matrix<int>::are_congruent (m_1, m_2));

    yLab::Matrix<int> m_3 {3, 3};
    EXPECT_FALSE (yLab::Matrix<int>::are_congruent (m_1, m_3));

    yLab::Matrix<int> m_4 {2, 2};
    EXPECT_TRUE  (yLab::Matrix<int>::are_congruent (m_1, m_4));
}

TEST (Other_Methods, Is_Square)
{
    yLab::Matrix<int> m_1 {2, 2};
    EXPECT_TRUE (m_1.is_square());

    yLab::Matrix<int> m_2 {5, 2};
    EXPECT_FALSE (m_2.is_square());
}

TEST (Other_Methods, Transposition)
{
    yLab::Matrix<int> m_1 = {{1, 2, 3},
                             {4, 5, 6},
                             {7, 8, 9}};
    yLab::Matrix<int> m_1_T = {{1, 4, 7},
                               {2, 5, 8},
                               {3, 6, 9}};
    m_1.transpose();
    EXPECT_TRUE (m_1 == m_1_T);

    yLab::Matrix<int> m_2 = {{1,  2,  3,  4},
                             {5,  6,  7,  8},
                             {9, 10, 11, 12}};
    yLab::Matrix<int> m_2_T = {{1, 5,  9},
                               {2, 6, 10},
                               {3, 7, 11},
                               {4, 8, 12}};
    m_2.transpose();
    EXPECT_TRUE (m_2 == m_2_T);

    yLab::Matrix<int> m_3 = {{ 1,  2,  3},
                             { 4,  5,  6},
                             { 7,  8,  9},
                             {10, 11, 12}};
    auto m_3_copy = m_3;
    EXPECT_TRUE (m_3 == m_3_copy.transpose().transpose());
}
