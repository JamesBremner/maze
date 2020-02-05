class Cell
{
public:
    bool top, down, right, left;
};

class cMaze
{

    std::vector< std::vector< Cell > > myMaze;

public:

    void read( const std::string& fname );

    std::vector< std::string >  displayText();

    /** Display using ascii text in graphical window
        Method never returns until window is closed
    */
    void displayWindow();
};
