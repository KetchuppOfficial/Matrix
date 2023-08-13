#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "random_matrix.hpp"

int main (int argc, char *argv[])
{
    std::string test_dir {argv[1], strlen (argv[1])};
    std::string ans_dir  {argv[2], strlen (argv[2])};

    int n_matrices = std::atoi (argv[3]);
    size_t size    = std::atoi (argv[4]);
    int max_det    = std::atoi (argv[5]);

    std::random_device rd;
    std::mt19937_64 gen (rd());
    std::uniform_int_distribution<int> det (-max_det, max_det);

    for (auto i = 1; i <= n_matrices; ++i)
    {
        auto actual_det = det (gen);
        yLab::Random_Matrix<int> matrix {size, actual_det};

        auto test_i = std::to_string (i);

        std::string test_name {"test_"};
        test_name += test_i;
        std::ofstream test_file {test_dir + test_name};

        test_file << size << std::endl;
        test_file << matrix;

        std::string ans_name {"answer_"};
        ans_name += test_i;
        std::ofstream ans_file {ans_dir + ans_name};

        ans_file << actual_det << std::endl;
    }

    return 0;
}
