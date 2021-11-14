#ifndef FM_LIB_NODE_H_
#define FM_LIB_NODE_H_

#include<string>
#include<sstream>
#include<iostream>

namespace fm {
    class Node
    {
    protected:
        Node *next;
        Node *prev;
        unsigned long cell_id;
        bool is_head;
        int current_gain; // if head, then stores the number of elements it is pointing to
    public:
        Node(unsigned long cell_id, int gain, Node *prev = nullptr, Node *next = nullptr, bool is_head = false);
        Node(const Node &node);
        ~Node();

        void setNext(Node* next);
        void setPrev(Node* prev);
        Node* getNext();
        Node* getPrev();

        long getCellID();
        void setCellID(unsigned long cell_id);

        void setCurrentGain(int gain);
        int getCurrentGain();

        std::string toString() const;

        bool isHead();

        void print();

    };

}
#endif // FM_LIB_NODE_H_