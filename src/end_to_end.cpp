#include <iostream>
#include <vector>
#include <cassert>

#include "matrix.hpp"

int main ()
{
    size_t size;
    std::cin >> size;
    assert (std::cin.good ());

    std::vector<int> buffer;
    buffer.reserve(size * size);

    for (auto i = 0; i != size * size; ++i)
    {
        int num;
        std::cin >> num;
        assert (std::cin.good());

        buffer.push_back (num);
    }

    Linear_Algebra::Matrix<int> matrix {size, size, buffer.begin(), buffer.end()};

    std::cout << matrix.determinant() << std::endl;
    
    return 0;
}
