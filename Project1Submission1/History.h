#ifndef HISTORY_H

#define HISTORY_H

#include "globals.h"
//put the class here
class History
{
	public:
		History(int nRows, int nCols);
		bool record(int r, int c);
		void display() const;
	private:
		//char** hist;
		char hist[MAXROWS][MAXCOLS] = { 0 };
		int h_rows;
		int h_cols;
};


#endif // for HISTORY_H

