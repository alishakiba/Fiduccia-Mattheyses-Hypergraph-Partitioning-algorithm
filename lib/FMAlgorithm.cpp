#include "FMAlgorithm.hpp"

#include <cstdlib>



bool fm::FMAlgorithm::is_balance_criterion_satisfied(char from) {
    int size = from == 'a' ? this->size_A - 1 : this->numOfCells - this->size_A - 1;
    if (this->numOfCells * this->ratio - this->k <= size
            && size <= this->numOfCells * this->ratio + this->k) {
            // we can move from A without violating the balance criterion
            return true;
    }
    else {
        // the balancing criterion is violated if we move a cell
        return false;
    }
}

void fm::FMAlgorithm::initial_partition(const std::set<int>* partition_A = nullptr) {
    // initialize the partition
    if (partition_A == nullptr) {
        // taking care of the balancing criterion
        int max_size_of_A = this->numOfCells * this->ratio + this->k;
        
        // randomly partition the data
        for (int i = 0; i < this->numOfCells; i++) {
            if (this->dis(this->gen) % 2 == 0 && this->size_A < max_size_of_A) {
                this->partition[i] = 'a';
                this->size_A++;
            }
            else {
                this->partition[i] = 'b';
            }
        }
        // check if the balancing criterion is satisfied
        if (this->size_A != max_size_of_A) {
            // move some cells from B to A to make the balance criterion satisfied
            int num_of_cells_to_move = max_size_of_A - this->size_A;
            int current_b_cell_to_move = 0;
            for (int i = 0; i < num_of_cells_to_move; i++) {
                while (this->partition[current_b_cell_to_move] != 'b') {
                    current_b_cell_to_move++;
                }
                this->partition[current_b_cell_to_move] = 'a';
            }
        }
    }
    else {
        // if the partition is given, we use it
        for (int i = 0; i < this->numOfCells; i++) {
            this->partition[i] = 'b';
        }
        for (auto cell : *partition_A) {
            this->partition[cell] = 'a';
            this->size_A++;
        }
    }
}
