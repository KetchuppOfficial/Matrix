#include <gtest/gtest.h>
#include <vector>

#include "matrix.hpp"

using namespace Linear_Algebra;

TEST (Constructors, Basic_Ctor)
{
    constexpr auto n_rows = 10;
    constexpr auto n_cols = 9;
    constexpr auto size = n_rows * n_cols;
    Matrix<int> m_1 {n_rows, n_cols};
    const int *arr_1 = new int[size]{};
    EXPECT_TRUE (std::memcmp (m_1.data (), arr_1, sizeof (arr_1)) == 0);

    Matrix<int> m_2 {3, 2, 7};
    const int arr_2[6] = {7, 7, 7, 7, 7, 7};
    EXPECT_TRUE (std::memcmp (m_2.data (), arr_2, sizeof (arr_2)) == 0);

    #ifdef COMPILATION_FAIL
    struct S {};
    Matrix<S> fail {2, 8};
    #endif // COMPILATION_FAIL
}

TEST (Constructors, Iterator_Ctor)
{
    std::vector<int> too_small = {1, 2};
    Matrix<int> m_3 {2, 2, too_small.begin(), too_small.end()};
    const int arr_3[] = {1, 2, 0, 0};
    EXPECT_TRUE (std::memcmp (m_3.data (), arr_3, sizeof (arr_3)) == 0);

    std::vector<int> perfect_fit = {3, 4, 5, 6};
    Matrix<int> m_4 {2, 2, perfect_fit.begin(), perfect_fit.end()};
    const int arr_4[] = {3, 4, 5, 6};
    EXPECT_TRUE (std::memcmp (m_4.data (), arr_4, sizeof (arr_4)) == 0);
    
    std::vector<int> too_big = {7, 8, 9, 10, 11, 12};
    Matrix<int> m_5 {2, 2, too_big.begin(), too_big.end()};
    const int arr_5[] = {7, 8, 9, 10};
    EXPECT_TRUE (std::memcmp (m_5.data (), arr_5, sizeof (arr_5)) == 0);
}
