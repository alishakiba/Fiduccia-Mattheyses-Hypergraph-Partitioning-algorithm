#ifndef FM_LIB_NODE_H_
#define FM_LIB_NODE_H_

namespace fm {
    class Node
    {
    protected:
        Node *next;
        Node *prev;
        bool head_node;
        unsigned long cell_id;
    public:
        Node(unsigned long cell_id, Node *prev = nullptr, Node *next = nullptr);
        Node(const Node &node);
        ~Node();

        void setNext(Node* next);
        void setPrev(Node* prev);
        Node* getNext();
        Node* getPrev();

        long getCellID();
        void setCellID(unsigned long cell_id);

    };
    
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

    void Node::setNext(const Node* next) {
        this->next = next;
    }

    void Node::setPrev(const Node* prev) {
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
#endif // FM_LIB_NODE_H_