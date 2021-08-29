#include "ScoreList.h"

ScoreList::ScoreList()      // Create an empty score list.
{
	
}

bool ScoreList::add(unsigned long score)
{
	if (0 <= score && score <= 100)
	{
		scores.insert(score);
		return true;
	}
	else 
		return false;
}
// If the score is valid (a value from 0 to 100) and the score list
// has room for it, add it to the score list and return true.
// Otherwise, leave the score list unchanged and return false.

bool ScoreList::remove(unsigned long score)
{
	return scores.erase(scores.find(score));
}
// Remove one instance of the specified score from the score list.
// Return true if a score was removed; otherwise false.

int ScoreList::size() const  // Return the number of scores in the list.
{
	return scores.size();
}

unsigned long ScoreList::minimum() const
{
	if (size() == 0)
		return NO_SCORE;

	unsigned long min;
	unsigned long current;
	scores.get(0, min);
	for (int i = 0; i < size(); i++)
	{
		scores.get(i, current);
		if (min > current)
			min = current;
	}
	return min;
}
// Return the lowest-valued score in the score list.  If the list is
// empty, return NO_SCORE.

unsigned long ScoreList::maximum() const
{
	if (size() == 0)
		return NO_SCORE;

	unsigned long max;
	unsigned long current;
	scores.get(0, max);
	for (int i = 0; i < size(); i++)
	{
		scores.get(i, current);
		if (max < current)
			max = current;
	}
	return max;
}
// Return the highest-valued score in the score list.  If the list is
// empty, return NO_SCORE.