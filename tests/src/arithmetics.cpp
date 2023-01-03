#include <gtest/gtest.h>
#include "matrix.hpp"

TEST (Arithmetics, Sum)
{
    yLab::Matrix<int> first = {{1, 2, 3},
                               {4, 5, 6},
                               {7, 8, 9}};

    yLab::Matrix<int> second = {{4, 2,  1},
                                {7, 5, -8},
                                {1, 3,  6}};
    
    yLab::Matrix<int> result = {{ 5,  4,  4},
                                {11, 10, -2},
                                { 8, 11, 15}};

    EXPECT_TRUE (first + second == result);

    first += second;
    EXPECT_TRUE (first == result);
}

TEST (Arithmetics, Difference)
{
    yLab::Matrix<int> first = {{1, 2, 3},
                               {4, 5, 6},
                               {7, 8, 9}};

    yLab::Matrix<int> second = {{4, 2,  1},
                                {7, 5, -8},
                                {1, 3,  6}};
    
    yLab::Matrix<int> result = {{-3, 0,  2},
                                {-3, 0, 14},
                                { 6, 5,  3}};

    EXPECT_TRUE (first - second == result);

    first -= second;
    EXPECT_TRUE (first == result);
}

TEST (Arithmetics, Multiplication_By_Number)
{
    yLab::Matrix<int> m = {{3, 8, 0, 9},
                           {1, -3, 5, 7}};

    yLab::Matrix<int> result = {{-9, -24,   0, -27},
                                {-3,   9, -15, -21}};
                            
    EXPECT_TRUE (m * (-3) == result);
    EXPECT_TRUE (-3 * m == result);

    m *= -3;
    EXPECT_TRUE (m == result);
}

TEST (Arithmetics, Division_By_Number)
{
    yLab::Matrix<int> m = {{3, 8, 0, 9},
                           {1, -3, 5, 7}};

    yLab::Matrix<int> result = {{-1, -4,  0, -4},
                                { 0,  1, -2, -3}};
                            
    EXPECT_TRUE (m / (-2) == result);

    m /= -2;
    EXPECT_TRUE (m == result);
}

TEST (Arithmetics, Product)
{
    yLab::Matrix<int> first = {{1, 2, 3},
                               {4, 5, 6},
                               {7, 8, 9}};

    yLab::Matrix<int> second = {{4, 2,  1},
                                {7, 5, -8},
                                {1, 3,  6}};

    yLab::Matrix<int> result = {{21, 21,  3},
                                {57, 51,  0},
                                {93, 81, -3}};
    
    EXPECT_TRUE (product (first, second) == result);
}
