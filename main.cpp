#include<iostream>
#include "FMAlgorithm.h"
using namespace std;

int main(int argc, char **argv) {
    if (argc != 6)
    {
        cerr << "Usage: " << argv[0] << " input_file_name output_file_name ratio k MAX_ITER" << endl;
        return 1;
    }
    auto ratio = stod(argv[3]);
    FMAlgorithm fma(ratio, stoi(argv[5]), stoi(argv[4]));
    fma.run(argv[1], argv[2]);
    return 0;
}