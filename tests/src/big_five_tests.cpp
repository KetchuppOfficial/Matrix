#include <gtest/gtest.h>
#include <vector>

#include "matrix.hpp"

using namespace Matrix;

TEST (Constructors, Basic_Ctor)
{
    constexpr auto size = 10;
    constexpr auto size_sq = size * size;
    Sq_Matrix<int> m_1 {size};
    const int *arr_1 = new int[size_sq]{};
    EXPECT_TRUE (std::memcmp (m_1.data (), arr_1, size_sq) == 0);

    Sq_Matrix<int> m_2 {2, 6};
    const int arr_2[] = {6, 6, 6, 6};
    EXPECT_TRUE (std::memcmp (m_2.data (), arr_2, sizeof (arr_2)) == 0);

    #ifdef COMPILATION_FAIL
    struct S {};
    Sq_Matrix<S> fail {2};
    #endif // COMPILATION_FAIL
}

TEST (Constructors, Iterator_Ctor)
{
    std::vector<int> too_small = {1, 2};
    Sq_Matrix<int> m_3 {2, too_small.begin(), too_small.end()};
    const int arr_3[] = {1, 2, 0, 0};
    EXPECT_TRUE (m_3.size () == 2);
    EXPECT_TRUE (std::memcmp (m_3.data (), arr_3, sizeof (arr_3)) == 0);

    std::vector<int> perfect_fit = {3, 4, 5, 6};
    Sq_Matrix<int> m_4 {2, perfect_fit.begin(), perfect_fit.end()};
    const int arr_4[] = {3, 4, 5, 6};
    EXPECT_TRUE (m_4.size () == 2);
    EXPECT_TRUE (std::memcmp (m_4.data (), arr_4, sizeof (arr_4)) == 0);
    
    std::vector<int> too_big = {7, 8, 9, 10, 11, 12};
    Sq_Matrix<int> m_5 {2, too_big.begin(), too_big.end()};
    const int arr_5[] = {7, 8, 9, 10};
    EXPECT_TRUE (m_5.size () == 2);
    EXPECT_TRUE (std::memcmp (m_5.data (), arr_5, sizeof (arr_5)) == 0);
}
