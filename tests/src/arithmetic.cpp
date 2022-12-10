#include <gtest/gtest.h>
#include "matrix.hpp"

TEST (Arithmetic, Product)
{
    yLab::Matrix<int> first = {{1, 2, 3},
                               {4, 5, 6},
                               {7, 8, 9}};

    yLab::Matrix<int> second = {{1, 4, 7},
                                {2, 5, 8},
                                {3, 6, 9}};

    yLab::Matrix<int> prod = {{14,  32,  50},
                              {32,  77, 122},
                              {50, 122, 194}};
    
    EXPECT_TRUE (product (first, second) == prod);
}
