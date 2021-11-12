#include "lib/FMAlgorithm.hpp"

int main(int argc, char *argv[]) {
    fm::FMAlgorithm fm(0.5, 2, 0);
    fm.run(argv[1], argv[2]);
    return 0;
}