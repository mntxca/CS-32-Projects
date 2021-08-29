#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

//Complete, runs in O(1) time like spec
StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	// TODO
	m_y = 0;
	m_x = 0;
	m_text.push_back("");
	m_iter = m_text.begin();
	preventUndoLoop = false;
}

//Complete at O(N) complexity, like spec
StudentTextEditor::~StudentTextEditor()
{
	// TODO
	//This is O(N) because the entire stack will be freed automatically
	//DO NOT FREE UNDO POINTER OBJECT HERE
}

//Complete at O(M+N), like spec
bool StudentTextEditor::load(std::string file) {
	// TODO
	ifstream infile(file);
	if (!infile)
	{
		cerr << "Error: Cannot open the file!" << endl;
		return false;
	}
	//everything after this point means that the file was loaded
	reset(); //O(M)
	string currentLine;
	char c;
	while (infile.get(c))
	{
		if (c == '\r')
			; //do nothing, just essentialy skip it and do not add to currentLine
		else if (c == '\n')
			m_text.push_back("");
		else
		{
			getline(infile, currentLine);
			m_text.push_back(c + currentLine); //add c to currentLine otherwise
		}
	}
	m_x = 0;
	m_y = 0;
	m_iter = m_text.begin();
	return true;
}

//Complete at O(M) like spec
bool StudentTextEditor::save(std::string file) {
	ofstream outfile(file);
	if (!outfile)
	{
		cerr << "Error: Cannot create" << file << " !" << endl;
		return false;
	}
	
	for (list<string>::iterator it = m_text.begin(); it != m_text.end(); it++)
	{
		outfile << *it << '\n';
	}
	return true;
}

//Complete at O(N+U) like spec
void StudentTextEditor::reset() {
	// TODO
	m_text.clear(); //O(N)
	getUndo()->clear(); //O(U)
}

//Complete at O(1) complexity, like spec
void StudentTextEditor::move(Dir dir) {
	// TODO
	//m_iter = m_text.begin();
	if ((m_y == 0) && (dir== Dir::UP))
		return;
	list<string>::iterator original = m_iter;
	if (dir == Dir::UP)
	{
		if (m_y <= 0)
			;//do nothing if at top row already
		else
		{
			m_y--;
			m_iter--;
			if (m_iter->size() < m_x)
				m_x = (*m_iter).size(); 
			//if line is current line is longer, cursor should be at end of the new line you are at
		}
	}
	else if (dir == Dir::DOWN)
	{
		m_iter++;
		if (m_iter == m_text.end())
			m_iter--;//do nothing if at bottom row already by undoing iter++
		else
		{
			m_y++; //iter has already added one
			if (m_iter->size() < m_x)
				m_x = (*m_iter).size();
		}
	}
	else if (dir == Dir::LEFT)
	{
		if (m_x == 0)
		{
			if (m_y > 0)
			{
				original--;
				m_x = (*original).size();
				m_y--;
				m_iter--;
			}
		}
		else
			m_x--;
	}
	else if (dir == Dir::RIGHT)
	{
		if (m_x == (*m_iter).size())
		{
			original++;
			if (original == m_text.end())
				return;//you are at the end already, so you can't go down or do anything to move right
			m_x = 0;
			move(Dir::DOWN);
		}
		else
			m_x++;
	}
	else if (dir == Dir::HOME)
	{
		m_x = 0;
	}
	else if (dir == Dir::END)
	{
		m_x = m_iter->size(); //size function for string is O(1)
	}
}

//Complete at O(L) complexity, or O(L1 + L2) when merging, like spec
void StudentTextEditor::del() {
	// TODO
	int lineLength = m_iter->size(); //string size function is O(1)
	if (m_x >= 0 && m_x < lineLength)
	{
		char ch = (*m_iter)[m_x];
		m_iter->erase(m_x, 1); //O(L1) erases the character at the x positon
		if(!preventUndoLoop)
			getUndo()->submit(Undo::DELETE, m_y, m_x, ch); //after delete processed
	}
	else if (m_x == lineLength)
	{
		list<string>::iterator next = m_iter;
		next++;
		if (next == m_text.end())
			return;//this is just after the last character on last line, no merge to do
		//merging is below
		*next = *m_iter + *next; //O(L1 + L2)
		m_iter = m_text.erase(m_iter); //O(L1)
		if (!preventUndoLoop)
			getUndo()->submit(Undo::JOIN, m_y, m_x, 0); //after delete processed
	}
}

//Complete at O(L) complexity, or O(L1 + L2) when merging, like spec
void StudentTextEditor::backspace() {
	// TODO
	if (m_x > 0) //this part works
	{
		char ch = (*m_iter)[m_x - 1];
		m_iter->erase(m_x - 1, 1); //erases one before
		m_x--; //then decrements the x position
		if (!preventUndoLoop)
			getUndo()->submit(Undo::DELETE, m_y, m_x, ch); //after backspace processed
	}
	else if (m_x == 0 || m_iter->size() == 0)
	{
		if (m_y == 0)
			return; //if you are already at the top row, you can't merge
		//this will merge two lines together
		list<string>::iterator previous = m_iter;
		previous--; //so now previous really is at the previous Node
		int previousLength = previous->size(); //O(L2)
		*m_iter = *previous + *m_iter;//O(L1+L2)
		m_iter = m_text.erase(previous); //O(L2)
		m_y--;
		m_x = previousLength;
		if (!preventUndoLoop)
		getUndo()->submit(Undo::JOIN, m_y, m_x, 0); //after backspace processed
	}
}

//Complete at O(L) complexity, like spec
void StudentTextEditor::insert(char ch) {
	// TODO
	if (ch == '	') 
	{
		m_iter->insert(m_x, "    "); //O(L), inserts 4 spaces
		for (int i = 1; i <= 4; i++)
		{
			m_x++;
			if (!preventUndoLoop)
			getUndo()->submit(Undo::INSERT, m_y, m_x, ' '); //after insert processed
		}
	}
	else
	{
		string s;
		s += ch;
		if (m_x >= (*m_iter).size())
			*m_iter += s;
		else
			m_iter->insert(m_x, s); //O(L), inserts the character
		m_x++;
		if (!preventUndoLoop)
		getUndo()->submit(Undo::INSERT, m_y, m_x, ch); //after inserted processed
	}
}

//Complete at O(L) complexity, like spec
void StudentTextEditor::enter() {
	// TODO
	if (!preventUndoLoop)
	getUndo()->submit(Undo::SPLIT, m_y, m_x, 0); //BEFORE enter processed
	string leftSide = m_iter->substr(0, m_x); //O(L)
	string rightSide = m_iter->substr(m_x); //O(L)
	*m_iter = rightSide;
	m_iter = m_text.insert(m_iter, leftSide); // probably O(L)
	m_iter++;
	m_x = 0;
	m_y++;
}


//Complete at O(1) complexity, like spec asks
void StudentTextEditor::getPos(int& row, int& col) const {
	// TODO
	row = m_y; col = m_x; 
}

//Complete at O(oldR + abs(current row number - startRow) + numRows*L) complexity, like spec asks
int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	if (startRow < 0 || numRows < 0 || startRow > m_text.size()) //startRow in this case is after the document ends
		return -1;
	lines.clear(); //O(oldR), this should clear everything in the vector
	list<string>::const_iterator const_iter = m_iter;
	
	if (m_y < startRow)
	{
		for (int i = m_y; i < startRow; i++) //O(abs(current row number - startRow)), like spec asks
			const_iter++; //get the iterator to the correct startRow
	}
	else
	{
		for (int i = m_y; i > startRow; i--) //O(abs(current row number - startRow)), like spec asks
		{
			const_iter--; //get the iterator to the correct startRow
		}
	}

	int i = 0;
	//const_iter != m_text.end();
	
	for (; i < numRows && const_iter != m_text.end(); const_iter++) //O(numRows*L)
	{
		lines.push_back(*const_iter);
		i++;
	}

	return i; // TODO
}

void StudentTextEditor::undo() {
	// TODO
	//get(int& row, int& col, int& count, std::string& text)
	int row;
	int col;
	int count;
	string text;
	Undo::Action action = getUndo()->get(row, col, count, text);
	if (action == Undo::Action::ERROR)
		return; //basically, do nothing in this case
	if (m_y < row)
	{
		for (int i = m_y; i < row; i++) //O(abs(current row number - startRow)), like spec asks
			m_iter++; //get the iterator to the correct startRow
	}
	else
	{
		for (int i = m_y; i > row; i--) //O(abs(current row number - startRow)), like spec asks
		{
			m_iter--; //get the iterator to the correct startRow
		}
	}
	preventUndoLoop = true;
	switch (action)
	{
	case Undo::INSERT:
	{
		m_x = col;
		m_y = row;
		//for insert, the iterator does not have to move as it is on the same line
		int length = text.size();
		for (int i = 0; i < length; i++)
		{
			insert(text[i]);
		}
		m_x = col;
		//m_x = col - (length - 1);
		m_y = row;
		break;
	}
	case Undo::DELETE:
	{
		m_x = col - 1;
		m_y = row;
		//for delete, the iterator does not have to move as it is on the same line

		for (int i = 0; i < count; i++)
		{
			del();
			//m_x++;
		}
		break;
	}
	case Undo::JOIN:
	{
		m_x = col;
		m_y = row;
		//m_iter--;
		del();
		break;
	}
	case Undo::SPLIT:
	{
		m_x = col;
		m_y = row;
		enter();
		m_x = col;
		m_y = row;
		m_iter--;
		break;
	}
	default:
		break; //you have an invalid action, so do nothing I guess
	}
	preventUndoLoop = false;
}
