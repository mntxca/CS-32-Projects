#include "History.h"
#include "globals.h"
using namespace std;

History::History(int nRows = MAXROWS, int nCols = MAXCOLS)
    :h_rows(nRows), h_cols(nCols)
{
}


bool History::record(int r, int c)
{
    if (!(r >= 1 && r <= h_rows && c >= 1 && c <= h_cols))
        return false;
    if (hist[r][c] == 0)
        hist[r][c] = 'A';
    else if (hist[r][c] == 'Z');
    else
        hist[r][c]++;
    return true;
}

void History::display() const
{
    // Position (row,col) in the city coordinate system is represented in
    int r, c;

    // Draw the grid
    clearScreen();
    for (r = 1; r <= h_rows; r++)
    {
        for (c = 1; c <= h_cols; c++)
        {
            if (hist[r][c] == 0)
                cout << '.';
            else
                cout << hist[r][c];
        }
        cout << endl;
    }
    cout << endl;
}