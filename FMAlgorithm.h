#ifndef BUCKET_LIST_STRUCTURE_HEADER_H
#define BUCKET_LIST_STRUCTURE_HEADER_H

#include "BucketListStructure.h"
#include "BucketListNode.h"

class FMAlgorithm
{
private:
    bool* partition; // true: partition A, false: partition B
    bool* free_cells; // true: is free, false: is locked
    int number_of_cells;
    int number_of_nets;
    int* gain; // the curret value of gain of a cell
    // distribution[0][i] : A(i)
    // distribution[1][i] : B(i)
    int* distribution[2]; 
    double ratio;
public:
    FMAlgorithm(int number_of_cells, int number_of_nets, double ratio) {
        this->number_of_cells = number_of_cells;
        this->number_of_nets = number_of_nets;
        // TODO: initial partition
        this->gain = new int[this->number_of_cells];
        this->distribution[0] = new int[this->number_of_nets];
        this->distribution[1] = new int[this->number_of_nets];
        this->ratio = ratio;
    }
    /*
        #pins
        #nets
        #cells

    */
    void read_input() {

    }
    // this method computes the initial distribution 
    // of the nets in the graph w.r.t. the initial 
    // bi-partition
    void compute_initial_distribution() {

    }
    void compute_initial_gain() {
        for(int i =0; i < this->number_of_cells; ++i) {
            this->gain[i] = 0;

        }
    }
    ~FMAlgorithm();
};

#endif 