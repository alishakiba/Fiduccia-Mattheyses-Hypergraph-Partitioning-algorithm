#ifndef BUCKET_LIST_NODE_HEADER_H
#define BUCKET_LIST_NODE_HEADER_H
class BucketListNode
{
private:
    // which cell does thisnode corresponds to
    int cell_number;
    BucketListNode* prev;
    BucketListNode* next;
public:
    BucketListNode(int cell_number, BucketListNode* prev=nullptr, BucketListNode* next=nullptr) {
        this->prev = prev;
        this->next = next;
        this->cell_number = cell_number;
    }

    int get_cell_number() {
        return this->cell_number;
    }

    int set_cell_number(int cell_number) {
        this->cell_number = cell_number;
    }

    BucketListNode* get_prev() {
        return this->prev = prev;
    }

    BucketListNode* get_next() {
        return this->next = next;
    }

    BucketListNode* set_prev(BucketListNode* prev) {
        this->prev = prev;
    }

    BucketListNode* set_next(BucketListNode* next) {
        this->next = next;
    }
};
#endif