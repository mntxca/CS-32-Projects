
#include "newSequence.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// To test a Sequence of unsigned long, leave the #define line commented out;
// to test a Sequence of string, remove the "//".

// #define TEST_WITH_STRING

#ifdef TEST_WITH_STRING
const ItemType VAL[3] = { "injera", "lavash", "tortilla" };
#else // assume unsigned long
const ItemType VAL[3] = { 999, 10, 20 };
#endif

void test()
{
    Sequence s;
    s.insert(0, VAL[1]);
    s.insert(0, VAL[2]);
    assert(s.size() == 2);
    ItemType x = VAL[0];
    assert(s.get(0, x) && x == VAL[2]);
    assert(s.get(1, x) && x == VAL[1]);

    Sequence a(1000);   // a can hold at most 1000 items
    Sequence b(5);      // b can hold at most 5 items
    Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
    ItemType v = 3;

    // No failures inserting 5 items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v) != -1);

    // Failure if we try to insert a sixth item into b
    assert(b.insert(v) == -1);

    // When two Sequences' contents are swapped, their capacities are
    // swapped as well:
    a.swap(b);
    assert(a.insert(v) == -1 && b.insert(v) != -1);
}
/*
void stringTest() {
    
   Sequence s;
    assert(s.empty() == true);
    assert(s.insert(1, "hello") == -1);
    assert(s.empty() == true);
    assert(s.insert(0, "hello") == 0);
    assert(s.empty() == false);
    assert(s.insert("kungpao") == 1);
    assert(s.insert(0, "byebye") == 0);
    assert(s.set(0, "hello") == true);
    assert(s.insert(1, "hello") == 1);
    assert(s.remove("hello") == 3);
    Sequence t;
    assert(t.insert("byebye") == 0);
    s.swap(t);
    assert(s.find("byebye") == 0);
    assert(t.find("kungpao") == 0);
    
}*/

void unSignedLongTest()
{
    Sequence s;
    assert(s.empty() == true);
    assert(s.insert(1, 3) == -1);
    assert(s.empty() == true);
    assert(s.insert(0, 3) == 0);
    assert(s.empty() == false);
    assert(s.insert(4) == 1);
    assert(s.insert(0, 5) == 0);
    assert(s.set(0, 3) == true);
    assert(s.insert(1, 3) == 1);
    assert(s.remove(3) == 3);
    Sequence t;
    assert(t.insert(5) == 0);
    s.swap(t);
    assert(s.find(5) == 0);
    assert(t.find(4) == 0);
}


int main()
{
    test();
    //stringTest();
    unSignedLongTest();
    cout << "Passed all tests" << endl;
}