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
        for(int i = 0; i < this->number_of_cells; ++i) {
            this->cells[i] = nullptr;
        }
        this->gains = new BucketListNode*[pmax * 2 + 1];
        for(int i = 0; i < this->pmax * 2 + 1; ++i) {
            BucketListNode *dummy_node = new BucketListNode(-1);
            this->gains[i] = dummy_node;
        }
        this->max_gain_index = 0;
    }
    ~BucketListStructure() {
        for(int i = 0; i < this->number_of_cells; ++i)
            delete this->cells[i];
        for(int i = 0; i < 2 * this->pmax + 1; ++i)
            delete this->gains[i];
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
    // this is the gain of the newly added cell
    // we'll check whether we need to update the
    // max_gain_index value
    void update_max_gain_index_after_add(int gain) {
        if (gain_to_index(gain) > this->max_gain_index) {
            this->max_gain_index = gain_to_index(gain);
        }
    }
    // adding a cell to front of gains list
    void add_cell_to_front(int cell, int gain) {
        if (this->cells[cell] != nullptr) {
            // we had it previously, so lets just change it.
            auto node = this->cells[cell];
            this->remove_cell_from_gain(node, gain);
            this->add_cell_to_front_gain(node, gain);
        }
        else {
            // it was not here, so lets just add it
            BucketListNode *node = new BucketListNode(cell);
            this->add_cell_to_front_gain(node, gain);
            this->cells[cell] = node;
        }
        this->update_max_gain_index_after_add(gain);
    }
    // adding a cell node to front of gains list
    void add_cell_to_front_gain(BucketListNode* node, int gain) {   
        auto dummy_head = this->gains[gain];
        if (dummy_head->get_next() == nullptr) {
            // it is empty
            dummy_head->set_next(node);
            dummy_head->set_prev(node);
            node->set_next(dummy_head);
            node->set_prev(dummy_head);
        }
        else {
            // it is not empty
            auto first_node = dummy_head->get_next();
            node->set_next(first_node);
            node->set_prev(dummy_head);
            first_node->set_prev(node);
            dummy_head->set_next(node);
        }
    }
    void add_cell_to_end(int cell, int gain) {
        if (this->cells[cell] != nullptr) {
            // we had it previously, so lets just change it.
            auto node = this->cells[cell];
            this->remove_cell_from_gain(node, gain);
            this->add_cell_to_end_gain(node, gain);
        }
        else {
            // it was not here, so lets just add it
            BucketListNode *node = new BucketListNode(cell);
            this->add_cell_to_end_gain(node, gain);
            this->cells[cell] = node;
        }
        this->update_max_gain_index_after_add(gain);
    }
    void add_cell_to_end_gain(BucketListNode* node, int gain) {   
        auto dummy_head = this->gains[gain];
        if (dummy_head->get_next() == nullptr) {
            // it is empty
            dummy_head->set_next(node);
            dummy_head->set_prev(node);
            node->set_next(dummy_head);
            node->set_prev(dummy_head);
        }
        else {
            // it is not empty
            auto last_node = dummy_head->get_prev();
            node->set_prev(last_node);
            node->set_next(dummy_head);
            last_node->set_next(node);
            dummy_head->set_prev(node);
        }
    }
    void remove_cell_from_gain(BucketListNode* node, int gain) {   
        auto prev = node->get_prev();
        auto next = node->get_next();
        if (prev->get_cell_number() == -1 && next->get_cell_number() == -1) {
            // we are removing from beginning of the list
            // and the list would be empty afterwards
            prev->set_next(nullptr);
            prev->set_prev(nullptr);

            node->set_next(nullptr);
            node->set_prev(nullptr);

            update_max_gain_index_after_remove(gain);
        }
        else if (prev->get_cell_number() == -1) {
            // we are removing from beginning of the list
            prev->set_next(next);
            next->set_prev(prev);

            node->set_next(nullptr);
            node->set_prev(nullptr);
        }
        else if (next->get_cell_number() == -1) {
            // we are removing from the end of the list
            next->set_prev(prev);
            prev->set_next(next);

            node->set_next(nullptr);
            node->set_prev(nullptr);
        }
        else {

        }
    }
    // precondition: the gains[gain] is of length 0 after removal of node
    void update_max_gain_index_after_remove(int gain) {
        if (gain_to_index(gain) >= this->max_gain_index) {
            auto gain_index = gain_to_index(gain);
            for(int i = gain_index; i >= 0; --i) {
                auto node = this->gains[i];
                if (node->get_next() != nullptr) {
                    this->max_gain_index = i;
                    break;
                }
            }
        }
    }
};

#endif 