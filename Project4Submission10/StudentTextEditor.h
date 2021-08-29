#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <list>
using namespace std;
class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo); //Complete, runs in O(1) time like spec
	~StudentTextEditor(); //Complete at O(N) complexity, like spec
	bool load(std::string file); //Complete at O(M+N), like spec
	bool save(std::string file); //Complete at O(M) like spec
	void reset(); 
	void move(Dir dir); //Complete at O(1) complexity, like spec
	void del(); //Complete at O(L) complexity, or O(L1 + L2) when merging, like spec
	void backspace(); //Complete at O(L) complexity, or O(L1 + L2) when merging, like spec
	void insert(char ch); //Complete at O(L) complexity, like spec
	void enter(); //Complete at O(L) complexity, like spec
	void getPos(int& row, int& col) const; //Complete at O(1) complexity, like spec asks
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const; //Complete at O(oldR + abs(current row number - startRow) + numRows*L) complexity, like spec asks
	void undo();

private:
	list<string> m_text;
	int m_y;
	int m_x;
	list<string>::iterator m_iter;
	bool preventUndoLoop;
	//do not store undo pointer here, as that is done in the TextEditor base class already
};

#endif // STUDENTTEXTEDITOR_H_
