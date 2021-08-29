#include <string>
#include <iostream>
#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
private:
    int m_row;
    int m_col;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack; //declare a stack of Coords
    Coord start(sr, sc);
    coordStack.push(start);
    maze[sr][sc] = 'a';
    while (!coordStack.empty())
    {
        Coord current = coordStack.top();
        coordStack.pop();
        if (current.c() == ec && current.r() == er)
            return true; //because now maze is solved as the current location is at the end
        if ((current.r() - 1 != -1) && maze[current.r() - 1][current.c()] == '.') //For moving north/up
        {
            coordStack.push(Coord(current.r() - 1, current.c()));
            maze[current.r() - 1][current.c()] = 'a';
        }
        
        if ((current.c() + 1 != nCols) && maze[current.r()][current.c() + 1] == '.') //For moving east/right
        {
            coordStack.push(Coord(current.r(), current.c() + 1));
            maze[current.r()][current.c() + 1] = 'a';
        }
        if ((current.r() + 1 != nRows) && maze[current.r() + 1][current.c()] == '.') //For moving south/down
        {
            coordStack.push(Coord(current.r() + 1, current.c()));
            maze[current.r() + 1][current.c()] = 'a';
        }
        if ((current.c() - 1 != -1) && maze[current.r()][current.c() - 1] == '.') //For moving west/left
        {
            coordStack.push(Coord(current.r(), current.c() - 1));
            maze[current.r()][current.c() - 1] = 'a';
        }
       
    }
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX", //0
        "X........X", //1
        "XX.X.XXXXX", //2
        "X..X.X...X", //3
        "X..X...X.X", //4
        "XXXX.XXX.X", //5
        "X.X....XXX", //6
        "X..XX.XX.X", //7
        "X...X....X", //8
        "XXXXXXXXXX"  //9
    };

    if (pathExists(maze, 10, 10, 6, 4, 1, 1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    string maze2[10] = {
        "XXXXXXXXXX", //0
        "X........X", //1
        "XX.X.XXXXX", //2
        "X..X.X...X", //3
        "X..X...X.X", //4
        "XXXX.XXX.X", //5
        "X.X....XXX", //6
        "X..XX.XX.X", //7
        "X...X....X", //8
        "XXXXXXXXXX"  //9
    };
    if (pathExists(maze2, 10, 10, 8, 1, 1, 1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    string maze3[10] = {
        "XXXXXXXXXX", //0
        "X........X", //1
        "XX.X.XXXXX", //2
        "X..X.X...X", //3
        "X..X...X.X", //4
        "XXXX.XXX.X", //5
        "X.X....XXX", //6
        "X..XX.XX.X", //7
        "X...X....X", //8
        "XXXXXXXXXX"  //9
    };
    if (pathExists(maze3, 10, 10, 7, 8, 1, 1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
