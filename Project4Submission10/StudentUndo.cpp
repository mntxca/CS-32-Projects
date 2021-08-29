#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}


void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// TODO
	bool emptyStack = undoStack.empty();
	if (!(emptyStack))
	{ 
		UndoObject* top = &(undoStack.top());
		if (top->getAction() == action)
		{
			if (action == INSERT && top->isConsecutive(col, row))
			{
				//top.addLetter(ch);
				top->deleteAnotherForInsert(ch, col); //increment the deleteCount
				return;
			}
			else if (action == DELETE && top->isConsecutive(col, row))
			{
				top->deleteAnotherForDeleteOrBackspace(ch, col);
				return;
			}
		}
	}
	//else 
	

		switch(action)
		{ 
			case INSERT:
			{
				UndoObject latestAction = UndoObject(action, row, col, ch);
				undoStack.push(latestAction);
				break;
			}
			case DELETE:
			{
				UndoObject latestAction = UndoObject(action, row, col, ch);
				undoStack.push(latestAction);
				break;
			}
			case JOIN:
			{
				UndoObject latestAction = UndoObject(action, row, col, 0);
				undoStack.push(latestAction);
				break;
			}
			case SPLIT:
			{
				UndoObject latestAction = UndoObject(action, row, col, 0);
				undoStack.push(latestAction);
				break;
			}
			default:
				break; //you have an invalid action, so do nothing I guess
		}

	
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
	// TODO	
	if (undoStack.empty())
		return Undo::Action::ERROR;
	UndoObject top = undoStack.top();
	switch (top.getAction())
	{
	case INSERT:
	{
		row = top.getRow();
		col = top.getCol();
		count = top.getDeleteCount();
		
		undoStack.pop();
		return Undo::Action::DELETE;
		break;
	}
	case DELETE:
	{
		row = top.getRow();
		col = top.getCol();
		count = 1;
		text = top.getChanges();
		undoStack.pop(); 
		return Undo::Action::INSERT;
		break;
	}
	case JOIN:
	{
		row = top.getRow();
		col = top.getCol();
		count = 1;
		undoStack.pop();
		return Undo::Action::SPLIT;
		break;
	}
	case SPLIT:
	{
		row = top.getRow();
		col = top.getCol();
		count = 1;
		undoStack.pop();
		return Undo::Action::JOIN;
		break;
	}
	default:
		return Action::ERROR;
		break; //you have an invalid action, so return an error
	}
}

void StudentUndo::clear() {
	// TODO
	while (!(undoStack.empty()))
		undoStack.pop(); //this should get rid of all of the objects
}
