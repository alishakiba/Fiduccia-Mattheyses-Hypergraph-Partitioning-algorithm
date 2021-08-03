#ifndef BUCKET_LIST_STRUCTURE_HEADER_H
#define BUCKET_LIST_STRUCTURE_HEADER_H

#include "BucketListNode.h"

class BucketListStructure
{
private:
    int number_of_cells;
    int pmax;
    BucketListNode** cells;
    BucketListNode** gains;
    int max_gain_index;
public:
    BucketListStructure(int number_of_cells, int pmax) {
        this->number_of_cells = number_of_cells;
        this->pmax = pmax;
        this->cells = new BucketListNode*[number_of_cells];
        this->gains = new BucketListNode*[pmax * 2 + 1];
        this->max_gain_index = 0;
    }
    ~BucketListStructure() {
        for(int i = 0; i < this->number_of_cells; ++i)
            delete this->cells[i];
        delete[] this->cells;
        delete[] this->gains;
    }
    int index_to_gain(int index) {
        return index - this->pmax;
    }
    int gain_to_index(int gain) {
        return gain + this->pmax;
    }
    int max_gain_index() {
        return this->max_gain_index;
    }
    int max_gain() {
        return this->index_to_gain(this->max_gain_index);
    }
    bool move_cell_in(int cell) {
        // cell is moved to this partition
    }
    bool move_cell_out(int cell) {
        // cell is moved to the other partition
    }
};
#endif 