#ifndef Sequence_H

#define Sequence_H
#include <string>
using ItemType = std::string; //TheTypeOfElementGoesHere;

class Sequence
{
public:
    Sequence() { nodeCount = 0; head = nullptr; newCount = 0; deleteCount = 0; }
    bool empty() const { return (nodeCount == 0); } //if nodeCount is 0, then it is empty and thus true, otherwise false
    int size() const { return nodeCount; }
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);

    //Destructor
    ~Sequence();
    
    //Copy Constructor
    Sequence(const Sequence& other); 
    
    //Assignment Operator
    Sequence& operator=(const Sequence& rhs);
private:
    struct Node {
        ItemType item; //is the item actually important to the doubly-linked list
        Node* next;
        Node* previous;
    };
    int nodeCount; //keeps track of how many nodes there are in the doubly-linked list
    Node* head; //head of the doubly-linked list
    int newCount;
    int deleteCount;

    int uncheckedInsert(int pos, const ItemType& value);
};

//Non-member Functions
int subsequence(const Sequence& seq1, const Sequence& seq2);

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif // for Sequence_H