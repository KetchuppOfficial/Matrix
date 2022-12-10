#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

#include "matrix.hpp"

TEST (Constructors, Basic_Ctor)
{
    constexpr auto n_rows = 10;
    constexpr auto n_cols = 9;
    constexpr auto size = n_rows * n_cols;
    yLab::Matrix<int> m_1 {n_rows, n_cols};
    const int *arr_1 = new int[size]{};
    EXPECT_TRUE (std::equal (m_1.data(), m_1.data() + m_1.size(), arr_1));

    yLab::Matrix<int> m_2 {3, 2, 7};
    const int arr_2[6] = {7, 7, 7, 7, 7, 7};
    EXPECT_TRUE (std::equal (m_2.data (), m_2.data() + m_2.size(), arr_2));

    #ifdef COMPILATION_FAIL
    struct S {};
    Matrix<S> fail {2, 8};
    #endif // COMPILATION_FAIL
}

TEST (Constructors, Iterator_Ctor)
{
    std::vector<int> too_small = {1, 2};
    yLab::Matrix<int> m_3 {2, 2, too_small.begin(), too_small.end()};
    const int arr_3[] = {1, 2, 0, 0};
    EXPECT_TRUE (std::equal (m_3.data(), m_3.data() + m_3.size(), arr_3));

    std::vector<int> perfect_fit = {3, 4, 5, 6};
    yLab::Matrix<int> m_4 {2, 2, perfect_fit.begin(), perfect_fit.end()};
    const int arr_4[] = {3, 4, 5, 6};
    EXPECT_TRUE (std::equal (m_4.data(), m_4.data() + m_4.size(), arr_4));
    
    std::vector<int> too_big = {7, 8, 9, 10, 11, 12};
    yLab::Matrix<int> m_5 {2, 2, too_big.begin(), too_big.end()};
    const int arr_5[] = {7, 8, 9, 10};
    EXPECT_TRUE (std::equal (m_5.data (), m_5.data() + m_5.size(), arr_5));
}

#if 0
TEST (Constructors, Double_Initializer_List)
{
    try
    {
        Matrix<int> matrix = {{1, 2, 3, 4},
                              {5, 6, 7}};

        EXPECT_TRUE (false);
    }
    catch (Il_Il_Ctor_Fail &expt)
    {
        EXPECT_TRUE (true);
    }

    try
    {
        Matrix<int> matrix = {{1, 2, 3, 4},
                              {5, 6, 7, 8}};

        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8};
        
        EXPECT_EQ (std::equal (matrix.data(), matrix.data() + matrix.size(), vec.begin()));
    }
    catch (Il_Il_Ctor_Fail &expt)
    {
        EXPECT_TRUE (false);
    }

}
#endif
