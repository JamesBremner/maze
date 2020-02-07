#include <iostream>
#include <vector>
#include "cMaze.h"

using namespace std;

int main( int argc, char* argv[] )
{
    cMaze theMaze;

    theMaze.config().Parse( argc, argv );

    theMaze.read();

    theMaze.generate();

    for( auto& s : theMaze.displayText() )
        cout << s << "\n";

    //theMaze.displayWindow();

    theMaze.svg("maze.svg");

}


