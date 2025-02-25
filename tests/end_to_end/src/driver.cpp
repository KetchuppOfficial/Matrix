#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "matrix.hpp"

int main() try
{
    #ifdef INTEGER
    using elem_type = long long;
    #else
    using elem_type = double;
    #endif

    std::size_t size;
    std::cin >> size;
    if (!std::cin.good())
        throw std::runtime_error{"reading size of matrix failed"};

    yLab::Matrix<elem_type> matrix{size, size, std::istream_iterator<elem_type>{std::cin},
                                   std::istream_iterator<elem_type>{}};
    std::cout << matrix.determinant() << std::endl;

    return 0;
}
catch (const std::exception &e)
{
    std::cerr << "Caught an instance of " << typeid(e).name() << "\nwhat(): " << e.what() << '\n';
    return 1;
}
catch (...)
{
    std::cerr << "Caught an unknown exception\n";
    return 1;
}
