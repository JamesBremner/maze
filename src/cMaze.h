class cConfig
{
public:

    std::string inputfilename;
    bool fgenerate;
    int nrows;
    int ncols;
    enum class eGenAlgo {
        none,
        binary,
        recursive
    } myGenAlgo;

    cConfig()
    : inputfilename("")
    , fgenerate( false )
    , nrows( 8 )
    , ncols( 8 )
    {

    }

    void Parse( int argc, char *argv[] );
};

class cCell
{
public:
    bool top, down, right, left;    ///< true if wall present

    /// CTOR - build walls all around cell
    cCell( bool w = true )
    : top( w )
    , down( w )
    , right( w )
    , left( w )
    {

    }
};

class cMaze
{
public:

    void read();

    /// generate random solveable maze using selected algorithm
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

    cConfig& config()
    {
        return myConfig;
    }

private:
    std::vector< std::vector< cCell > > myMaze;
    cConfig myConfig;
    void generate_binary_tree();

    /// Start maze generation using recursive division algorithm
    void generate_recursive_init();

    /// Recursive division generator
    void generate_recursive( int x, int y, int w, int h );
};
