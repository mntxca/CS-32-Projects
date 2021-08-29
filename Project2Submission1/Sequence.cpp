#include "Sequence.h"
using namespace std;

int Sequence::insert(int pos, const ItemType& value) {
	if (0 <= pos && pos <= size())
	{
		//do the insertion and return pos
		uncheckedInsert(pos, value);
		return pos;
	}
	else
		return -1;
}
// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one higher than they were at before.
// Return pos if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// e.g., because it's implemented using a fixed-size array.)  Otherwise,
// leave the sequence unchanged and return -1.  Notice that
// if pos is equal to size(), the value is inserted at the end.

int Sequence::insert(const ItemType& value)
{
	int pos = 0;
	//find the position pos where the input will go in the sequence
	for (Node* p = head; (p != nullptr) && (value > p->item); p = p->next)
		pos++; 
	uncheckedInsert(pos, value);
	return pos;
}
// *****There is no 'a priori' limit on the maximum numbers of items in the sequence, so this function should never return -1*****
//Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item in position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.

int Sequence::uncheckedInsert(int pos, const ItemType& value) {
	int position = 0;
	Node* p = head;
	Node* tempOneBeforePosNode;
	Node* tempOneAfterPosNode;
	Node* insert = new Node; newCount++; //the node that will actually be inserted

	if (pos == 0)
	{
		tempOneBeforePosNode = nullptr; //before zeroth node comes a nullptr
		tempOneAfterPosNode = head; //after zeroth node comes what used to be the head
		head = insert; //the new zeroth node is now the head
	}
	else
	{
		//sets position to one before pos
		for (; (p != nullptr); p = p->next)
		{
			if (pos == (position + 1))
				break;
			position++;
		}
		tempOneBeforePosNode = p;

		if (tempOneBeforePosNode != nullptr) //avoid dereferencing nullptr
			tempOneAfterPosNode = tempOneBeforePosNode->next;
		else
			tempOneAfterPosNode = nullptr; //if p is nullptr, the next node also must not exist, so also must be nullptr
	}
	//Instantiating all the variables of the inserted node
	insert->item = value;
	insert->previous = tempOneBeforePosNode;
	insert->next = tempOneAfterPosNode;

	//now make the pointers of before and after the inserted node point to insert (if before and after aren't nullptr)
	if (tempOneBeforePosNode != nullptr)
		tempOneBeforePosNode->next = insert;
	if (tempOneAfterPosNode != nullptr)
		tempOneAfterPosNode->previous = insert;

	nodeCount++;
	return pos;
}

bool Sequence::erase(int pos) {
	if (0 <= pos && pos < size())
	{
		//do the erase and return true
		int position = 0;
		Node* p = head;
		//put pointer p at position pos in the sequence
		for (; (p != nullptr); p = p->next)
		{
			if (pos <= position)
				break;
			position++;
		} //at end of the loop p is at position pos, where erasing needs to be done
		if (pos == 0 && p != nullptr) //make the head the new zeroth index
			head = p->next;
		if (p != nullptr) //don't do anything if p is the nullptr, can't erase no Node, this case actually shouldn't happen
		{
			if (p->previous != nullptr)
				(p->previous)->next = p->next;
			if (p->next != nullptr)
				(p->next)->previous = p->previous;
			delete p; //don't forget to delete the erased node
			p = nullptr;
			deleteCount++;
			nodeCount--;
		}
		return true;
	}
	else
		return false;
}

int Sequence::remove(const ItemType& value) {
	int position = 0;
	int deleted = 0;
	//check every node's item to see if it is == to value, and erase those nodes
	for (Node* pointer = head; (pointer != nullptr); )
	{
		if (pointer == nullptr && pointer->next == nullptr)
			break;
		if (pointer->item == value) //match found, so erase...
		{
			pointer = pointer->next; //so next time code is run you don't point at the deleted Node deleted in erase in the next line
			erase(position);
			deleted++;
			//to avoid skipping the next node, you DO NOT increment the position and 'move forward' a node if you erase
		}
		else
		{
			//no erasing, so increment
			pointer = pointer->next;
			position++;
		}
	}
	return deleted;
}

bool Sequence::get(int pos, ItemType& value) const {
	if (0 <= pos && pos < size())
	{
		//go to the pos item, copy that item into value, and return true
		int position = 0;
		//"go to the pos item" is what the loop is for
		for (Node* p = head; (p != nullptr); p = p->next)
		{
			if (pos == position)
			{
				value = p->item; //"copy that item into value"
				return true;
			}
			position++;
		}
		return false;
	}
	else
		return false;
}

bool Sequence::set(int pos, const ItemType& value) {
	if (pos < 0 || pos >= size())
		return false;
	int position = 0;
	//loop is for going to the pos node 
	for (Node* p = head; (p != nullptr); p = p->next)
	{
		if (pos == position)//now at pos Node
		{
			p->item = value; //copy value into item
			return true;
		}
		//increment position after the if statement
		position++;
	}
	return false;
}

int Sequence::find(const ItemType& value) const
{
	int position = 0;
	//check every node's item to see if it is == to value, and at first item == value, return position
	for (Node* p = head; (p != nullptr); p = p->next)
	{
		if (p->item == value)//match found...
		{
			return position; //return where first match was
		}
		//increment position after the if statement
		position++;
	}
	return -1;
}

void Sequence::swap(Sequence& other)
{
	//for switching the heads of the doubly-linked lists
	Node* temp = other.head;
	other.head = head;
	head = temp;

	//for switching the nodeCounts
	int tempNodeCount = other.nodeCount;
	other.nodeCount = nodeCount;
	nodeCount = tempNodeCount;

	//for switching the newCounts
	int tempNewCount = other.newCount;
	other.newCount = newCount;
	newCount = tempNewCount;

	//for switching the deleteCounts
	int tempDeleteCount = other.deleteCount;
	other.deleteCount = deleteCount;
	deleteCount = tempDeleteCount;
}

//Destructor
Sequence::~Sequence()
{
	//needs to delete every node
	//start at 0th item, then move forward one item
	//delete previous (0th item), move forward one item
	//delete previous (1st item), move forward one item
	//repeat until next pointer is nullptr, in that case delete the current item
	Node* p = head;
	if (p == nullptr || size() == 0); //nothing to delete, as the sequence is empty, destructor is done
	else if ((p->next) == nullptr || size() == 1)
	{
		delete p; //and now the single item sequence has had its only item deleted, destructor is done
		p = nullptr;
		deleteCount++;
	}
	else
	{
		for (p = p->next; (p != nullptr); p = p->next)
		{
			if (deleteCount >= newCount) //you are done if you deleted as many (or more) nodes that you dynamically allocated
				break;

			delete (p->previous);
			deleteCount++;
			nodeCount--;
			p->previous = nullptr;
			if (p->next == nullptr)
			{
				delete p; // p then must already be at the last item in the sequence, delete p and destructor is done
				p = nullptr;
				deleteCount++;
				nodeCount--;
				break;
			}
		}
	}
}

//Copy Constructor
Sequence::Sequence(const Sequence& other)
{
	nodeCount = other.nodeCount; //keeps track of how many nodes there are in the doubly-linked list
	newCount = 0;
	deleteCount = 0;
	if (other.head != nullptr)
	{
		head = new Node; newCount++; //this is the new head node for newly constructed sequence
		head->previous = nullptr;
		head->item = other.head->item;
		Node* p = other.head;
		Node* newNode = head;
		for (; p != nullptr; p = p->next)
		{
			if (p->next != nullptr) //there is a next node in the other sequence
			{
				newNode->next = new Node; newCount++; //make a next node in this sequence if there's a corresponding next node in other sequence
				(newNode->next)->previous = newNode;
				(newNode->next)->item = (p->next)->item;
				newNode = newNode->next; //move forward a node in this sequence
			}
			else
				newNode->next = nullptr; //there is no next node in other sequence, so make next node in this sequence = nullptr
		}
	}
	else
		head = nullptr;
}

//Assignment Operator
Sequence& Sequence::operator=(const Sequence& rhs)
{
	if (this != &rhs)//if aliased then nothing to do
	{
		Sequence temp(rhs); //make copy of rhs
		swap(temp); //make this (lhs swap with temp, which is copy of rhs
	}
	return *this;
}

int subsequence(const Sequence& seq1, const Sequence& seq2) {
	if (seq2.empty())
		return -1;
	if (&seq1 == &seq2) //if seq1 and seq2 are references to the same sequence, return 0 
		return 0;
	ItemType temp;
	ItemType temp2;
	bool match = false;
	//go through seq1 and try to find match for first node's item of seq2
	for (int i = 0; i < seq1.size(); i++)
	{
		for (int j = 0; j < seq2.size();)
		{
			if (seq1.get(i + j, temp) == true && seq2.get(j, temp2) == true && temp == temp2) //if match found, check next node
			{
				j++;
				match = true;
			}
			else
			{
				match = false; //not a match/not a complete match, go to next node of seq1
				break;
			}
		}
		if (match) //if match then return the position it was found at
			return i;
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	//find which sequence is bigger and smaller and the lengths of both
	int m = seq1.size();
	int n = seq2.size();
	int lowerSize;
	int higherSize;
	int bigger;
	if (m < n)
	{
		lowerSize = m;
		higherSize = n;
		bigger = -1;
	}
	else if (m == n)
	{
		lowerSize = m;
		higherSize = m;
		bigger = 0;
	}
	else
	{
		lowerSize = n;
		higherSize = m;
		bigger = 1;
	}
	Sequence tempSeq; //if you make the entire result sequence in a temporary sequence and then make result = temp you guard against aliasing
	ItemType temp1;
	int positionSeq1 = 0; //position in seq1
	int positionSeq2 = 0; //position in seq2
	int position = 0; //position in result
	for (int i = 0; i < lowerSize; i++) //interleave up to when shorter sequence of sequences 1 and 2 ends, this should just alternate
	{
		//get and insert the corresponding item from sequence 1
		seq1.get(positionSeq1, temp1);
		tempSeq.insert(positionSeq1 + positionSeq2, temp1);
		positionSeq1++; position++;
		
		//get and insert the corresponding item from sequence 2
		seq2.get(positionSeq2, temp1);
		tempSeq.insert(positionSeq1 + positionSeq2, temp1);
		positionSeq2++;	position++;
	}
	if (bigger == -1) //finish the rest of adding elements of sequence 2 to the temporary sequence if sequence 2 is longer
	{
		for (int j = lowerSize; j < higherSize; j++)
		{
			seq2.get(positionSeq2, temp1);
			tempSeq.insert(position, temp1);
			positionSeq2++; position++;
		}
	}
	if (bigger == 1) //finish the rest of adding elements of sequence 1 to the temporary sequence if sequence 2 is longer
	{
		for (int j = lowerSize; j < higherSize; j++)
		{
			seq1.get(positionSeq1, temp1);
			tempSeq.insert(position, temp1);
			positionSeq1++; position++;

		}
	}
	result = tempSeq; //make the result sequence get the sequence of the temporary sequence to protect against aliasing
}