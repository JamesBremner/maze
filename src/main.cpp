#include <iostream>
#include <vector>

#include "cMaze.h"

using namespace std;

int main()
{
    cMaze theMaze;
    theMaze.read("maze2.txt");

    for( auto& s : theMaze.displayText() )
        cout << s << "\n";

    theMaze.displayWindow();

}


