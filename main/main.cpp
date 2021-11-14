#include "lib/FMAlgorithm.hpp"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] 
            << " <input_file> <output_file> [<max_iterations>] [<ratio>] [<k>] [<verbose>]" 
            << std::endl;
        return 1;
    }
    std::string input_file(argv[1]);
    std::string output_file(argv[2]);
    int max_iterations = (argc > 3) ? atoi(argv[3]) : 100;
    double ratio = (argc > 4) ? atof(argv[4]) : 0.1;
    int k = (argc > 5) ? atoi(argv[5]) : 10;
    bool verbose = (argc > 6) ? true : false;

    if (verbose) {
        std::cout << "input_file: " << input_file << std::endl;
        std::cout << "output_file: " << output_file << std::endl;
        std::cout << "max_iterations: " << max_iterations << std::endl;
        std::cout << "ratio: " << ratio << std::endl;
        std::cout << "k: " << k << std::endl;
        std::cout << "verbose: " << verbose << std::endl;
    }

    fm::FMAlgorithm fm(ratio, k, max_iterations, verbose);
    fm.run(input_file, output_file);

    return 0;
}