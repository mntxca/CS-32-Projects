#include "ScoreList.h"
#include <cassert>
#include <iostream>
using namespace std;


int main()
{
	ScoreList s;
	assert(s.add(70));
	assert(s.add(65));
	assert(s.add(100));
	assert(s.add(7));
	assert(!s.add(101));
	assert(!s.add(-1));
	assert(s.maximum() == 100);
	assert(s.remove(100));
	assert(s.maximum() == 70);
	assert(s.minimum() == 7);

	cerr << "passed" << endl;
}
