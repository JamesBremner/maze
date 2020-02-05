class cCell
{
public:
    bool top, down, right, left;    ///< true if wall present

    /// CTOR - build walls all around cell
    cCell()
    : top( true )
    , down( true )
    , right( true )
    , left( true )
    {

    }
};

class cMaze
{

    std::vector< std::vector< cCell > > myMaze;

public:

    void read( const std::string& fname );

    /// generate random solveable maze using binary tree algorithm
    void generate();

    std::vector< std::string >  displayText();

    /** write maze display to SVG file
        param[in] fn SVG output filename
    */
    void svg( const std::string& fn );

    /** Display using ascii text in graphical window
        Method never returns until window is closed
    */
    void displayWindow();
};
