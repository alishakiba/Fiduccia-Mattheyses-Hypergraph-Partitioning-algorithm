#include "Node.hpp"

namespace fm {
    
    Node::Node(unsigned long cell_id, Node *prev = nullptr, Node *next = nullptr)
        : next (next), prev (prev), cell_id (cell_id)
    {
    }

    Node::Node(const Node &node) 
        : Node(node.cell_id, node.prev, node.next)
    {
    }
    
    Node::~Node()
    {
        this->next = nullptr;
        this->prev = nullptr;
    }

    void Node::setNext(Node* next) {
        this->next = next;
    }

    void Node::setPrev(Node* prev) {
        this->prev = prev;
    }

    Node* Node::getNext() {
        return this->next;
    }
    
    Node* Node::getPrev() {
        return this->prev;
    }

    long Node::getCellID() {
        return this->cell_id;
    }
    
    void Node::setCellID(unsigned long cell_id) {
        this->cell_id = cell_id;
    }

}