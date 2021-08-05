#ifndef FM_ALGORITHM_HEADER_H
#define FM_ALGORITHM_HEADER_H

#include "BucketListStructure.h"
#include "BucketListNode.h"
#include <set>
#include <time.h>
#include <cstdlib>
using namespace std;

class FMAlgorithm
{
private:
    bool* partition; // true: partition A, false: partition B
    bool* free_cells; // true: is free, false: is locked
    int number_of_cells;
    int number_of_nets;
    int pmax;
    int* gain; // the curret value of gain of a cell
    // distribution[0][i] : A(i)
    // distribution[1][i] : B(i)
    int* distribution[2]; 
    double ratio;
    set<int>* nets_to_cells;
    set<int>* cells_to_nets;
    BucketListStructure* bucket_structure_a;
    BucketListStructure* bucket_structure_b;
    int size_A;
    int seed;
    int k; // balance tolerance
public:
    FMAlgorithm(int number_of_cells, int number_of_nets, double ratio, int pmax) {
        this->number_of_cells = number_of_cells;
        this->number_of_nets = number_of_nets;
        // TODO: initial partition
        this->gain = new int[this->number_of_cells];
        this->distribution[0] = new int[this->number_of_nets];
        this->distribution[1] = new int[this->number_of_nets];
        this->ratio = ratio;
        this->nets_to_cells = new set<int>[this->number_of_nets];
        this->cells_to_nets = new set<int>[this->number_of_cells];
        this->pmax = pmax;
        this->seed = time(0);
        srand(this->seed);
        /* */
        for(int i = 0; i < this->number_of_cells; ++i) 
            this->free_cells[i] = false;
    }
    void initial_partition(const set<int>* partition_A = nullptr) {
        if (partition_A == nullptr) {
            // do a random partition
            for(int i = 0; i < this->number_of_cells; ++i) {
                if (rand() / (double) RAND_MAX > 0.5) {
                    this->partition[i] = true;
                    this->size_A++;
                }
            }
        }
        else {
            for(auto cell: *partition_A) {
                this->partition[cell] = true;
            }
            this->size_A = partition_A->size();
        }
    }
    // this method computes the initial distribution 
    // of the nets in the graph w.r.t. the initial 
    // bi-partition
    void compute_initial_distribution() {
        for(int i = 0; i < this->number_of_nets; ++i) {
            int A = 0;
            int B = 0;
            for(auto cell: this->nets_to_cells[i]) {
                if (this->partition[cell]) {
                    A++;
                }
                else {
                    B++;
                }
            }
            this->distribution[0][i] = A;
            this->distribution[1][i] = B;
        }
    }
    // this method computes the initial gain 
    // of the nets in the graph w.r.t. the initial 
    // bi-partition
    void compute_initial_gain() {
        for(int i =0; i < this->number_of_cells; ++i) {
            // computing gain for cell i
            this->gain[i] = 0;
            for(auto net: cells_to_nets[i]) {
                // for each net which cell i belongs to 
                if (partition[i]) {
                    // cell i belongs to partition A,
                    // so F=A and T=B according to the 
                    // terminology of the paper
                    if (this->distribution[0][i] == 1) {
                        this->gain[i]++;
                    }
                    if (this->distribution[1][i] == 0) {
                        this->gain[i]--;
                    }
                }
                else {
                    // cell i belongs to partition B,
                    // so F=B and T=A according to the 
                    // terminology of the paper
                    if (this->distribution[1][i] == 1) {
                        this->gain[i]++;
                    }
                    if (this->distribution[0][i] == 0) {
                        this->gain[i]--;
                    }
                }
            }
            // adding cell i to the BucketListStructure for
            // corresponding partition
            if (partition[i]) {
                // cell i belongs to partition A
                this->bucket_structure_a->add_cell_to_end(i, this->gain[i]);
            }
            else {
                // cell i belongs to partition B
                this->bucket_structure_b->add_cell_to_end(i, this->gain[i]);
            }
        }
    }
    // move base cell and update
    void move_base_cell_and_update(int cell) {
        int T, F;
        if (this->partition[cell]) {
            // this cell belongs to partition A
            // so, F=A and T=B
            // lock the base cell
            T = 1;
            F = 0;
        }
        else {
            // this cell belongs to partition B
            // so, F=B and T=A
            // lock the base cell
            T = 0;
            F = 1;
        }
        // lock the base cell
        this->free_cells[cell] = false;
        // change the base cell's partition
        this->partition[cell] = false;
        for(auto net: this->cells_to_nets[cell]) {
            // check critical nets
            if (distribution[T][net] == 0) {
                // increment gains of all free cells on net 
                for(auto free_cell: nets_to_cells[net]) {
                    if (this->free_cells[free_cell]) {
                        // it is a free cell
                        this->gain[free_cell]++;
                        if (this->partition[free_cell]) {
                            // it belongs to partition A
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else {
                            // it belongs to partition B
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
            else if (distribution[T][net] == 1) {
                // decrement gain of the only free cell on partition T
                for(auto free_cell: nets_to_cells[net]) {
                    if (this->free_cells[free_cell]) {
                        if (this->partition[free_cell] && T == 0) {
                            // it belongs to partition A
                            this->gain[free_cell]--;
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else if (this->partition[free_cell] && T == 1) {
                            // it belongs to partition B
                            this->gain[free_cell]--;
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
            // change the net distribution to reflect the move
            this->distribution[F][net]--;
            this->distribution[T][net]++;
            // check critical nets after the move
            if (distribution[F][net] == 0) {
                // decrement gains of all free cells on net 
                for(auto free_cell: nets_to_cells[net]) {
                    if (this->free_cells[free_cell]) {
                        // it is a free cell
                        this->gain[free_cell]--;
                        if (this->partition[free_cell]) {
                            // it belongs to partition A
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else {
                            // it belongs to partition B
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
            else if (distribution[F][net] == 1) {
                // decrement gain of the only free cell on partition F
                for(auto free_cell: nets_to_cells[net]) {
                    if (this->free_cells[free_cell]) {
                        if (this->partition[free_cell] && F == 0) {
                            // it belongs to partition A
                            this->gain[free_cell]++;
                            this->bucket_structure_a->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                        else if (this->partition[free_cell] && F == 1) {
                            // it belongs to partition B
                            this->gain[free_cell]++;
                            this->bucket_structure_b->add_cell_to_front(free_cell, this->gain[free_cell]);
                        }
                    }
                }
            }
        }
    }
    // an iteration of the algorithm
    bool iterate() {
        // identifying the base cell
        int max_gain_a = this->bucket_structure_a->max_gain();
        int max_gain_b = this->bucket_structure_b->max_gain();
        if (max_gain_a > max_gain_b &&
            
        ) {

        }
        
    }
    bool is_balance_criterion_valid(int from = 0) {
        // from == 0 : A
        // from == 1 : B
        // check whether the balancing criterion can be maintained
        // simulting the move by considering the move from A to B
        int size = from == 0 ? this->size_A - 1 : this->number_of_cells - this->size_A - 1;
        if (this->number_of_cells * this->ratio - this->k <= size
            && size <= this->number_of_cells * this->ratio + this->k) {
            // we can move from A without violating the balance criterion
            return true;
        }
        else {
            // the balancing criterion is violated if we move a cell
            return false;
        }
    }
    ~FMAlgorithm();
};

#endif 