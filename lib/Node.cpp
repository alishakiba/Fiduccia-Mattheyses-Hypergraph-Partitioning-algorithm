#include "Node.hpp"

namespace fm {
    
    Node::Node(unsigned long cell_id, Node *prev, Node *next, bool is_head)
        : next (next), prev (prev), cell_id (cell_id), is_head (is_head)
    {
    }

    Node::Node(const Node &node) 
        : Node(node.cell_id, node.prev, node.next, node.is_head)
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

    bool Node::isHead() {
        return this->is_head;
    }

    std::string Node::toString() const{
        std::stringstream ss;
        ss << "Node:\n\tCell ID:" << this->cell_id 
            << "\n\tPrev ID:" << ((this->prev != nullptr) ? std::to_string(this->prev->getCellID()) : "null") << "\n\t"
            << "\n\tNext ID:" << ((this->next != nullptr) ? std::to_string(this->next->getCellID()) : "null")
            << "\n";
        return ss.str();
    }

}