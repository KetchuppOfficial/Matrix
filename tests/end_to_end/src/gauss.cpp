#include <iostream>
#include <iterator>

#include "matrix.hpp"

int main ()
{
    auto cin_iter = std::istream_iterator<double> (std::cin);
    size_t size = *cin_iter;

    yLab::Matrix<double> matrix {size, size, ++cin_iter, std::istream_iterator<double>()};
    std::cout << matrix.determinant() << std::endl;

    return 0;
}
