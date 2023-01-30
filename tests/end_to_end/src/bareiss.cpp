#include <iostream>
#include <iterator>

#include "matrix.hpp"

int main ()
{
    auto cin_iter = std::istream_iterator<long long> (std::cin);
    size_t size = *cin_iter;

    yLab::Matrix<long long> matrix {size, size, ++cin_iter, std::istream_iterator<long long>()};
    std::cout << matrix.determinant() << std::endl;
    
    return 0;
}
