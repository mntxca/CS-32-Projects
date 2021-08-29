//#include "Header.h"
//only submit what is below this line

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	if (sr == er && sc == ec)
		return true; //at end point, so return true
	maze[sr][sc] = 'a';
    bool up = false;
    bool right = false;
    bool down = false;
    bool left = false;

	//each of the four directions
    if ((sr - 1 != -1) && maze[sr - 1][sc] == '.') //For moving north/up
    {
        up = pathExists(maze, nRows, nCols, sr - 1, sc, er, ec);
        if (up)
            return true;
    }

    if ((sc + 1 != nCols) && maze[sr][sc + 1] == '.') //For moving east/right
    {
        right = pathExists(maze, nRows, nCols, sr, sc + 1, er, ec);
        if (right)
            return true;
    }
    if ((sr + 1 != nRows) && maze[sr + 1][sc] == '.') //For moving south/down
    {
        down = pathExists(maze, nRows, nCols, sr + 1, sc, er, ec);
        if (down)
            return true;
    }
    if ((sc - 1 != -1) && maze[sr][sc - 1] == '.') //For moving west/left
    {
        left = pathExists(maze, nRows, nCols, sr, sc - 1, er, ec);
        if (left)
            return true;
    }
    //no paths found
    return up || right || down || left;
}

//only submit what is above this line
