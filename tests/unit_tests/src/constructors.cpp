#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

#include "matrix.hpp"

TEST (Constructors, Basic_Ctor)
{
    constexpr auto n_rows = 10;
    constexpr auto n_cols = 9;

    yLab::Matrix<int> m {n_rows, n_cols};
    EXPECT_EQ (m.n_rows(), n_rows);
    EXPECT_EQ (m.n_cols(), n_cols);

    const int arr[n_rows * n_cols]{};
    EXPECT_TRUE (std::equal (m.begin(), m.end(), arr));
}

TEST (Constructors, Basic_Ctor_With_Initial_Value)
{
    constexpr auto n_rows = 3;
    constexpr auto n_cols = 2;

    yLab::Matrix<int> m {n_rows, n_cols, 7};
    EXPECT_EQ (m.n_rows(), n_rows);
    EXPECT_EQ (m.n_cols(), n_cols);

    const int arr[n_rows * n_cols] = {7, 7,
                                      7, 7,
                                      7, 7};
    EXPECT_TRUE (std::equal (m.begin(), m.end(), arr));
}

TEST (Constructors, Iterator_Ctor)
{
    constexpr auto n_rows = 2;
    constexpr auto n_cols = 2;
    
    std::vector<int> too_small = {1, 2};
    yLab::Matrix<int> m_1 {n_rows, n_cols, too_small.begin(), too_small.end()};
    EXPECT_EQ (m_1.n_rows(), n_rows);
    EXPECT_EQ (m_1.n_cols(), n_cols);
    const int arr_1[] = {1, 2,
                         0, 0};
    EXPECT_TRUE (std::equal (m_1.begin(), m_1.end(), arr_1));

    std::vector<int> perfect_fit = {3, 4, 5, 6};
    yLab::Matrix<int> m_2 {n_rows, n_cols, perfect_fit.begin(), perfect_fit.end()};
    EXPECT_EQ (m_2.n_rows(), n_rows);
    EXPECT_EQ (m_2.n_cols(), n_cols);
    const int arr_2[] = {3, 4,
                         5, 6};
    EXPECT_TRUE (std::equal (m_2.begin(), m_2.end(), arr_2));
    
    std::vector<int> too_big = {7, 8, 9, 10, 11, 12};
    yLab::Matrix<int> m_3 {n_rows, n_cols, too_big.begin(), too_big.end()};
    EXPECT_EQ (m_3.n_rows(), n_rows);
    EXPECT_EQ (m_3.n_cols(), n_cols);
    const int arr_3[] = {7, 8,
                         9, 10};
    EXPECT_TRUE (std::equal (m_3.begin(), m_3.end(), arr_3));
}

TEST (Constructors, Double_Initializer_List_Ctor)
{
    try
    {
        yLab::Matrix<int> m = {{1, 2, 3, 4},
                               {5, 6, 7}};

        EXPECT_TRUE (false);
    }
    catch (yLab::Il_Il_Ctor_Fail &expt)
    {
        EXPECT_TRUE (true);
    }

    try
    {
        yLab::Matrix<int> m = {{1, 2, 3, 4},
                               {5, 6, 7, 8}};
        
        EXPECT_EQ (m.n_rows(), 2);
        EXPECT_EQ (m.n_cols(), 4);

        const int arr[] = {1, 2, 3, 4,
                           5, 6, 7, 8};
        EXPECT_TRUE (std::equal (m.begin(), m.end(), arr));
    }
    catch (yLab::Il_Il_Ctor_Fail &expt)
    {
        EXPECT_TRUE (false);
    }
}

TEST (Constructors, Identity_Matrix_Ctor)
{
    constexpr auto n_rows = 3;
    constexpr auto n_cols = 4;

    auto m_1 = yLab::Matrix<int>::identity_matrix (n_rows, n_cols);
    EXPECT_EQ (m_1.n_rows(), n_rows);
    EXPECT_EQ (m_1.n_cols(), n_cols);

    const int arr_1[] = {1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0};
    EXPECT_TRUE (std::equal (m_1.begin(), m_1.end(), arr_1));

    auto m_2 = yLab::Matrix<int>::identity_matrix (n_cols, n_rows);
    EXPECT_EQ (m_2.n_rows(), n_cols);
    EXPECT_EQ (m_2.n_cols(), n_rows);

    const int arr_2[] = {1, 0, 0,
                         0, 1, 0,
                         0, 0, 1,
                         0, 0, 0};
    EXPECT_TRUE (std::equal (m_2.begin(), m_2.end(), arr_2));
}
