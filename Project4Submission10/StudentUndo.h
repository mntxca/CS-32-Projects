#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"

#include <stack>
using namespace std;

/*class UndoObject
{
public:
	UndoObject(Undo::Action actionType, int row1, int column, char ch, int deleteCount1 = 1)
		:action(actionType), row(row1), recentCol(column), col(column), deleteCount(deleteCount1)
	{
		string s = "";
		s += ch;
		changes = s;
	}
	UndoObject(Undo::Action actionType, int row1, int column, int deleteCount1 = 0)
		:action(actionType), row(row1), recentCol(column), col(column), changes(""), deleteCount(deleteCount1)
	{}
	void addLetter(char ch) { changes += ch; }
	void deleteAnotherForInsert(char ch, int mostRecentCol)
	{
		deleteCount++;
		changes += ch;
		recentCol = mostRecentCol;
	}
	void deleteAnotherForDeleteOrBackspace(char ch, int mostRecentCol) { 
		deleteCount++;
		if(recentCol == mostRecentCol){
			//this is delete
			//add this letter to the back of the string
			changes += ch;
			//recentCol = mostRecentCol; not needed as this already is that same
		}
		if (recentCol - 1 == mostRecentCol)
		{
			//this is backspace
			//add this letter to the front of the string
			changes = ch + changes;
			recentCol = mostRecentCol;
			col = mostRecentCol;
		}
		
		
	}//recentColOnlyUseForConsecutive++;
	int getDeleteCount(){ return deleteCount; }
	int getRow() { return row; }
	int getCol() { return col; }
	string getChanges() { return changes; }
	Undo::Action getAction() { return action; }
	bool isConsecutive(int x2, int y2)
	{
		int x1 = this->recentCol;
		int y1 = this->row;
		if (action == Undo::INSERT)
			return isConsecutiveForInsertion(x1, y1, x2, y2);
		else
			return isConsecutive(x1, y1, x2, y2);
	}
private:
	bool isConsecutive(int x1, int y1, int x2, int y2)
	{
		if (y1 == y2)
		{
			int diff = x1 - x2;
			if (0 <= diff && diff <= 1)
				return true;
			else
				return false;
		}
		else return false;
	}
	bool isConsecutiveForInsertion(int x1, int y1, int x2, int y2)
	{
		if (y1 == y2)
		{
			int diff = x1 - x2;
			if (diff == -1)
				return true;
			else
				return false;
		}
		else return false;
	}
	
	Undo::Action action; //number will be represented by action 
	int row;
	int recentCol;
	int col;
	string changes;
	int deleteCount;
};*/

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
	
	
	class UndoObject
	{
	public:
		UndoObject(Undo::Action actionType, int row1, int column, char ch, int deleteCount1 = 1)
			:action(actionType), row(row1), recentCol(column), col(column), deleteCount(deleteCount1)
		{
			string s = "";
			s += ch;
			changes = s;
		}
		UndoObject(Undo::Action actionType, int row1, int column, int deleteCount1 = 0)
			:action(actionType), row(row1), recentCol(column), col(column), changes(""), deleteCount(deleteCount1)
		{}
		void addLetter(char ch) { changes += ch; }
		void deleteAnotherForInsert(char ch, int mostRecentCol)
		{
			deleteCount++;
			changes += ch;
			recentCol = mostRecentCol;
		}
		void deleteAnotherForDeleteOrBackspace(char ch, int mostRecentCol) {
			deleteCount++;
			if (recentCol == mostRecentCol) {
				//this is delete
				//add this letter to the back of the string
				changes += ch;
				//recentCol = mostRecentCol; not needed as this already is that same
			}
			if (recentCol - 1 == mostRecentCol)
			{
				//this is backspace
				//add this letter to the front of the string
				changes = ch + changes;
				recentCol = mostRecentCol;
				col = mostRecentCol;
			}


		}//recentColOnlyUseForConsecutive++;
		int getDeleteCount() { return deleteCount; }
		int getRow() { return row; }
		int getCol() { return col; }
		string getChanges() { return changes; }
		Undo::Action getAction() { return action; }
		bool isConsecutive(int x2, int y2)
		{
			int x1 = this->recentCol;
			int y1 = this->row;
			if (action == Undo::INSERT)
				return isConsecutiveForInsertion(x1, y1, x2, y2);
			else
				return isConsecutive(x1, y1, x2, y2);
		}
	private:
		bool isConsecutive(int x1, int y1, int x2, int y2)
		{
			if (y1 == y2)
			{
				int diff = x1 - x2;
				if (0 <= diff && diff <= 1)
					return true;
				else
					return false;
			}
			else return false;
		}
		bool isConsecutiveForInsertion(int x1, int y1, int x2, int y2)
		{
			if (y1 == y2)
			{
				int diff = x1 - x2;
				if (diff == -1)
					return true;
				else
					return false;
			}
			else return false;
		}

		Undo::Action action; //number will be represented by action 
		int row;
		int recentCol;
		int col;
		string changes;
		int deleteCount;
	};
	stack<UndoObject> undoStack;
};

#endif // STUDENTUNDO_H_
