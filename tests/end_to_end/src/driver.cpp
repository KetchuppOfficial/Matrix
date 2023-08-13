#include <iostream>
#include <iterator>

#include "matrix.hpp"

int main ()
{
    #ifdef INTEGER
    using elem_type = long long;
    #else
    using elem_type = double;
    #endif

    auto cin_iter = std::istream_iterator<elem_type>{std::cin};
    std::size_t size = *cin_iter;

    yLab::Matrix<elem_type> matrix {size, size, ++cin_iter, std::istream_iterator<elem_type>{}};
    std::cout << matrix.determinant() << std::endl;

    return 0;
}
