#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "random_matrix.hpp"

int main (int argc, char *argv[])
{
    std::string test_dir {argv[1], strlen (argv[1])};
    int n_matrices = std::atoi (argv[2]);
    size_t max_size = std::atoi (argv[3]);
    double max_det = std::atof (argv[4]);

    std::random_device rd;
    std::mt19937_64 gen (rd());
    std::uniform_int_distribution<size_t> size (0, max_size - 1);
    std::uniform_real_distribution<double> det (-max_det, max_det);

    for (auto i = 0; i != n_matrices; ++i)
    {
        yLab::Random_Matrix<double> matrix {size (gen), det (gen)};
        
        std::string file_name {"/test_"};
        file_name += std::to_string (i);

        std::ofstream test_file {test_dir + file_name};
        test_file << matrix;
    }
    
    return 0;
}
