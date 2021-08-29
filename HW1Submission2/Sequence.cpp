#include "Sequence.h"

Sequence::Sequence()
{
	objCount = 0;
}

bool Sequence::empty() const  // Return true if the sequence is empty, otherwise false.
{
	if (objCount <= 0)
		return true;
	else
		return false;
}

int Sequence::size() const    // Return the number of items in the sequence.
{
	return objCount;
}

//should work, hasn't been tested
int Sequence::insert(int pos, const ItemType& value) 
{
	if (size() == DEFAULT_MAX_ITEMS)
		return -1;
	if (0 <= pos && pos <= size()) //makes sure pos is in bounds of the interesting elements or just one too high
	{

		for (int i = objCount; i >= (pos + 1); i--)
			arr[i] = arr[i - 1]; // do the assigning here (shifting the index after pos one to the right)

		arr[pos] = value; //should work

		objCount++;
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

//should work, hasn't been tested
int Sequence::insert(const ItemType& value)
{
	int p = 0;
	for (; p < size(); p++)
	{
		if (value <= arr[p])
			break;
	}

	return insert(p, value);
}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item in position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).

bool Sequence::erase(int pos)
{
	if (0 <= pos && pos < size()) //makes sure pos is in bounds
	{
		for (int i = pos; i < (objCount - 1); i++)
			arr[i] = arr[i + 1]; // do the assigning here (shifting the index after pos one to the right)
		objCount--; //one less thing in sequence now
		return true;
	}
	else 
		return false;
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed that item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value)
{
	int p = 0;
	int deleteCount = 0;
	for (; p < size(); p++)
	{
		if (value == arr[p])
		{
			erase(p);
			p--;
			deleteCount++;
		}
	}
	return deleteCount;
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const
{
	if (0 <= pos && pos < size())
	{
		value = arr[pos];
		return true;
	}
	else 
		return false;
}
// If 0 <= pos < size(), copy into value the item at position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value)
{
	if (0 <= pos && pos < size())
	{
		arr[pos] = value;
		return true;
	}
	else
		return false;
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const
{
	int p = 0;
	for (; p < size(); p++)
	{
		if (value == arr[p])
			return p;
	}
	return -1;
}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.

void Sequence::swap(Sequence& other)
{
	int shortLength;
	int longLength;
	bool isOtherLonger;

	if (size() >= other.size())
	{
		shortLength = other.size();
		longLength = size();
		isOtherLonger = false;
	}
	else
	{
		shortLength = size();
		longLength = other.size();
		isOtherLonger = true;
	}
	//shortLength has the value of the shorter size array

	ItemType temp;

	for (int i = 0; i < shortLength; i++)
	{
		temp = arr[i];
		arr[i] = other.arr[i];
		other.arr[i] = temp;
	}

	for (int j = shortLength; j < longLength; j++)
	{
		if (isOtherLonger)
		{
			arr[j] = other.arr[j];
		}
		else
		{
			other.arr[j] = arr[j];
		}
	}

	int temp1 = objCount;
	objCount = other.objCount;
	other.objCount = temp1;

}
// Exchange the contents of this sequence with the other one.