#include "BucketList.hpp"

fm::BucketList::BucketList(int pmax, int C) 
    : pmax (pmax), C (C), max_gain (-pmax), max_gain_index (-1), remaining_cells (0) {
    this->cell = new Node*[C];
    for(int i = 0; i < C; ++i) {
        this->cell[i] = nullptr;
    }
    this->gains = new Node*[2 * pmax + 1];
    for(int i = 0; i < 2 * pmax + 1; ++i) {
        this->gains[i] = new Node(i, 0, nullptr, nullptr, true);
    }
}

fm::BucketList::BucketList(BucketList& bl) {
    //
}

fm::BucketList::~BucketList() {
    for(int i = 0; i < C; ++i) {
        delete this->cell[i];
    }
    delete [] this->cell;
    for(int i = 0; i < 2 * pmax + 1; ++i) {
        delete this->gains[i];
    }
    delete [] this->gains;
}

int fm::BucketList::gain_2_index(int gain) {
    return gain + this->pmax;
}

int fm::BucketList::index_2_gain(int index) {
    return index - this->pmax;
}

void fm::BucketList::add_cell_(int cell_id, int gain) {
    if (this->cell[cell_id] != nullptr) {
        // this is an update operation, rather than adding a new one
        this->remove_cell_(cell_id);
        this->add_cell_(cell_id, gain);
    }
    else {
        // this is an add operation
        Node *node = new Node(cell_id, gain);
        this->cell[cell_id] = node;
        int gain_index = gain_2_index(gain);
        auto gain_head = this->gains[gain_index];
        if (gain_head->getPrev() != nullptr) {
            // the list is not empty
            // so add the node at the end of the list

            node->setNext(gain_head);

            auto prev_node = gain_head->getPrev();
            node->setPrev(prev_node);

            prev_node->setNext(node);

            gain_head->setPrev(node);
            gain_head->setCurrentGain(gain_head->getCurrentGain() + 1); // increment the # of cells in this gain linked list
        }
        else if (gain_head->getNext() == nullptr) {
            // the list is empty
            gain_head->setPrev(node);
            gain_head->setNext(node);
            gain_head->setCurrentGain(1);

            node->setNext(gain_head);
            node->setPrev(gain_head);
        }
        else {
            // this should not happen, so let's throw an error
            throw std::runtime_error("Error: cell_id: " + std::to_string(cell_id) + " gain: " + std::to_string(gain));
        }
        this->remaining_cells++;
    }
}

void fm::BucketList::remove_cell_(int cell_id) {
    auto node = this->cell[cell_id];
    if (node != nullptr) {
        auto node_prev = node->getPrev();
        auto node_next = node->getNext();
        if (node_prev->isHead() && node_next->isHead()) {
            // this is the only element in the list
            node_prev->setPrev(nullptr);
            node_prev->setNext(nullptr);
        }
        else {
            node_prev->setNext(node_next);
            node_next->setPrev(node_prev);
        }
        this->cell[cell_id] = nullptr;
        this->remaining_cells--;

        auto gain_head = this->gains[this->gain_2_index(node->getCurrentGain())];
        gain_head->setCurrentGain(gain_head->getCurrentGain() - 1); // decrement the # of cells in this gain linked list
        delete node;
    }
}

void fm::BucketList::add_cell(int cell_id, int gain) {
    Node *gain_head;
    bool gain_update_required = false;
    if (this->cell[cell_id] != nullptr) {
        gain_head = this->gains[this->gain_2_index(this->cell[cell_id]->getCurrentGain())];
        gain_update_required = true;
    }
    if (gain > this->max_gain) {
        // we need to take care of the max_gain
        this->max_gain = gain;
        this->max_gain_index = this->gain_2_index(gain);
        this->add_cell_(cell_id, gain);
    }
    else if (gain_update_required) {
        //as well as gain <= this->max_gain
        this->add_cell_(cell_id, gain);
        
        Node *gain_head = nullptr;
        int i = this->max_gain_index;
        
        for(i = this->max_gain_index;  this->gains[i]->getCurrentGain() <= 0; gain_head = this->gains[i], --i);

        this->max_gain = this->index_2_gain(i);
        this->max_gain_index = i;
    }
    else {
        // gain <= this->max_gain and there is no need to update max_gain
        // no need to taking care of the max_gain
        // so just we run the add algorithm
        this->add_cell_(cell_id, gain);
    }
}

void fm::BucketList::remove_cell(int cell_id) {
    auto node = this->cell[cell_id];
    if (node != nullptr) {
        Node *gain_head = node;
        
        do {
            gain_head = gain_head->getPrev();
        } while (!gain_head->isHead());
        int gain = this->index_2_gain(gain_head->getCellID());
        
        if (gain < this->max_gain) {
            // no need to taking care of the max_gain
            // so just we run the add algorithm
            this->remove_cell_(cell_id);
        }
        else if (gain == this->max_gain) {
            // we need to take care of the max_gain
            auto node_prev = node->getPrev();
            auto node_next = node->getNext();
            if (node_prev->isHead() && node_next->isHead()) {
                // this is the only element in the list
                this->remove_cell_(cell_id);
                if (this->remaining_cells > 0) {
                    while (this->max_gain_index >=0 && this->gains[this->max_gain_index]->getNext() == nullptr) {
                        this->max_gain_index--;
                        this->max_gain--;
                    }
                }
                else {
                    this->max_gain_index = -1; // just to show it is invalid
                    this->max_gain = -this->pmax;
                }
            }
        }
        else {
            // some error happened
            throw std::runtime_error("Error: cell_id: " + std::to_string(cell_id));            
        }
    }
    // else {
    //     nothing is needed to do
    // }
}

int fm::BucketList::getMaxGain() {
    return this->max_gain;
}

int fm::BucketList::getMaxGainCellID() {
    if (this->max_gain_index >= 0 && this->gains[this->max_gain_index] != nullptr 
            && this->gains[this->max_gain_index]->getNext() != nullptr) {
        return this->gains[this->max_gain_index]->getNext()->getCellID();
    }
    else {
        return -1;
    }
}

bool fm::BucketList::isEmpty() {
    return this->remaining_cells == 0;
}

int fm::BucketList::getRemainingCells() {
    return this->remaining_cells;
}

int fm::BucketList::getMaxNumberCells() {
    return this->C;
}


void fm::BucketList::print() {
    std::cout << "Gains: " << std::endl;
    for (int i = 0; i < 2 * this->pmax + 1; ++i) {
        std::cout << "Gain: " << this->index_2_gain(i) << ": ";
        if (this->gains[i] != nullptr) {
            auto node = this->gains[i]->getNext();
            while (node != nullptr && node->isHead() == false) {
                std::cout << node->getCellID() << " ";
                node = node->getNext();
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Cells: " << std::endl;
    for (int i = 0; i < this->C; ++i) {
        std::cout << "Cell: " << i << ": ";
        if (this->cell[i] != nullptr) {
            this->cell[i]->print();
        }
        else {
            std::cout << "nullptr" << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "Max Gain: " << this->max_gain << std::endl;
    std::cout << "Max Gain Cell ID: " << this->getMaxGainCellID() << std::endl;
    std::cout << "Remaining Cells: " << this->remaining_cells << std::endl;
    std::cout << "Max Number of Cells: " << this->C << std::endl;
}