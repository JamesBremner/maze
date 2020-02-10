#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <boost/program_options.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include "raven_graph.h"
#include "wex.h"
#include "cMaze.h"
using namespace std;


void cConfig::Parse( int argc, char *argv[] )
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("input",   po::value<std::string>(), "input file.")
    ("gen",     po::value<std::string>(), "Generate new maze. Specify algorithm: none, binary, recursive")
    ;

    // parse the command line
    po::variables_map vm;
    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch (exception& e)
    {
        cerr << "error: " << e.what() << "\n";
        exit(1);
    }

    if (vm.count("help"))
    {
        cout << desc << "\n";
        exit(1);
    }

    if( vm.count("input") )
    {
        inputfilename = vm["input"].as<std::string>();
    }

    if( vm.count("gen") )
    {
        std::string a = vm["gen"].as<std::string>();
        if( a[0] == 'b')
            myGenAlgo = eGenAlgo::binary;
        else if( a[0] == 'r' )
            myGenAlgo = eGenAlgo::recursive;
    }

}
void cMaze::generate()
{
    /* initialize random seed: */
    srand (time(NULL));

    switch( myConfig.myGenAlgo )
    {
    case cConfig::eGenAlgo::binary:
        generate_binary_tree();
        break;
    case cConfig::eGenAlgo::recursive:
        generate_recursive_init();
        break;
    default:
        break;
    }
    return;
}

void cMaze::generate_binary_tree()
{
    int row_count = 10;
    int col_count = 10;

    for( int row = 0; row < row_count; row++ )
    {
        vector< cCell > vCell;
        for( int col = 0; col < col_count; col++ )
        {
            cCell C;

            if( row == 0 )
            {
                C.left = false;
            }
            else if( col == 0 )
            {
                C.top = false;
            }
            else
            {
                int r = rand() % 2;
                if( r )
                    C.left = false;
                else
                    C.top = false;
            }
            vCell.push_back( C );
        }
        myMaze.push_back( vCell );
    }
    // select random exit
    if( rand() % 2 )
    {
        int row = rand() % row_count;
        myMaze[row][col_count-1].right = false;
    }
    else
    {
        int col = rand() & col_count   ;
        myMaze[row_count-1][col].down = false;
    }
}

void cMaze::generate_recursive_init()
{
    // construct initial chamber, maze with no walls except around edges
    for( int r = 0; r < myConfig.nrows; r++ )
    {
        std::vector< cCell > vr;
        for( int c = 0; c < myConfig.ncols; c++ )
        {
            cCell C( false );
            if( r == 0 )
                C.top = true;
            if( r == myConfig.nrows-1 )
                C.down = true;
            if( c == 0 )
                C.left = true;
            if( c == myConfig.ncols-1 )
                C.right = true;
            vr.push_back( C );
        }
        myMaze.push_back( vr );
    }
    // open entrance and exit in walls around edges
    myStartRow = rand()%myConfig.nrows;
    myEndRow   = rand()%myConfig.nrows;
    myMaze[myStartRow][0].left = false;
    myMaze[myEndRow][myConfig.ncols-1].right = false;

    generate_recursive( 0, 0, myConfig.ncols, myConfig.nrows );
}

void cMaze::generate_recursive( int x, int y, int w, int h )
{
    // check for recurion completed
    if( w == 1 && h == 1 )
        return;

    // raise four walls
    int newx = w / 2;
    int newy = h / 2;
    for( int r = 0; r < h; r++ )
    {
        myMaze[y+r][x+newx].left = true;
    }
    for( int c = 0; c < w; c++ )
    {
        myMaze[y+newy][x+c].top = true;
    }
    // open passage in three of four walls
    int wnop = rand() % 4;
    if( wnop != 0 )
        myMaze[y+newy][x+rand() % newx].top = false;
    if( wnop != 1 )
        myMaze[y+newy][x+newx+(rand()%(w-newx))].top = false;
    if( wnop != 2 )
        myMaze[y+rand()%newy][x+newx].left = false;
    if( wnop != 3 )
        myMaze[y+newy+(rand()%(h-newy))][x+newx].left = false;

    // recurse into four smaller chambers
    generate_recursive( x, y, newx, newy );
    generate_recursive( x + newx, y, w - newx, newy );
    generate_recursive( x, y + newy, newx, h - newy );
    generate_recursive( x + newx, y + newy, w - newx, h - newy );

}

void cMaze::read()
{
    if( ! myConfig.inputfilename.length() )
        return;

    ifstream in( myConfig.inputfilename );
    char str1[100], str2[100], str3[100];
    in.getline(str1, 100);
    in.getline(str2, 100);
    in.getline(str3, 100);
    int line = 1, cell = 1;
    while (!in.eof())
    {
        vector< cCell > vCell;

        cell = 1;
        cout << "line " << line << endl;
        int i = 0;
        bool top, down, right, left;
        top = down = right = left = false;
        int j = 0, k = 1;
        cout << "strlen = " << strlen(str1) << endl;
        while (i < (int)strlen(str1) - 1)
        {
            cCell C;
            top = down = right = left = false;
            if (str1[i] == '+')
                i++; //new cell
            if (str1[i] == '-')
                top = true;
            else if (str1[i] == ' ')
                top = false;
            i = i + 3; //wall above
            if (str2[j] == '|')
                left = true;
            else if (str2[j] == ' ')
                left = false;
            j = j + 4; //left wall
            if (str2[j] == '|')
                right = true;
            else if (str2[j] == ' ')
                right = false; //right wall
            if (str3[k] == ' ')
                down = false;
            else if (str3[k] == '-')
                down = true;
            k = k + 4; //wall below
            //cout << "cell = " << cell << endl;
            cell++;
//            cout << "Top :  " << top << "\t";
//            cout << "down :  " << down << endl;
//            cout << "right :  " << right << "\t";
//            cout << "left :  " << left << endl;
//            cout << endl << endl;
            C.top = top;
            C.down = down;
            C.right = right;
            C.left = left;
            vCell.push_back( C );
        }
        myMaze.push_back( vCell );

        strcpy(str1, str3);
        cout << str1 << endl;
        in.getline(str2, 100);
        in.getline(str3, 100);
        cout << str2 << endl;
        cout << str3 << endl;
        line++;
    }
}


vector< std::string > cMaze::displayText()
{
    cout << myMaze.size() << " rows " << myMaze[0].size() << " cols\n";
    vector< string > vss;
    stringstream s1, s2, s3;
    int kr = 0;
    for( auto& r : myMaze )
    {
        int kc = 0;
        for( auto& c : r )
        {
            s1 << "+";
            if( c.top )
                s1 << "---";
            else
                s1 << "   ";
            if( c.left )
                s2 << "|";
            else
                s2 << " ";
            if( onPath(kc,kr))
                s2 << " * ";
            else
                s2 << "   ";
            if( kc == myMaze[0].size()-1 )
            {
                s1 << "+";
                if( c.right )
                    s2 << "|";
                else
                    s2 << " ";
            }
            if( kr == myMaze.size() - 1 )
            {
                s3 << "+";
                if( c.down )
                    s3 << "---";
                else
                    s3 << "   ";
                if( kc == myMaze[0].size()-1 )
                    s3 << "+";
            }
            kc++;
        }

//        cout << s1.str() << "\n";
//        cout << s2.str() << "\n";
        vss.push_back( s1.str() );
        vss.push_back( s2.str() );
        s1.str("");
        s2.str("");
        if( kr == myMaze.size() - 1 )
        {
            //cout << s3.str() << "\n";
            vss.push_back( s3.str() );
        }
        kr++;
    }

    return vss;
}

void cMaze::displayWindow()
{
    wex::gui& form = wex::maker::make();;
    form.move({ 50,50,400,500});
    form.text("Maze");
    form.events().draw([&]( PAINTSTRUCT& ps )
    {
        RECT r( ps.rcPaint );
        for( auto& s : displayText() )
        {
            wex::shapes S( ps );
            S.textFontName("Courier");
            S.text( s, { r.left, r.top } );
            r.top += 15;
        }
    });
    form.show();
    form.run();
}

void cMaze::svg( const std::string& fn )
{
    int row_count =  myMaze.size()-1;
    int col_count = myMaze[0].size()-1;
    ofstream of(fn);

    of << "<svg width=\"" << (col_count+2)*50 << "\" height=\"" << (row_count+2)*50;
    of << "\" viewBox=\"" << -20 <<" "<< -20 <<" "<< (col_count+2)*50 <<" "<<(row_count+2)*50;
    of << "\" xmlns=\"http://www.w3.org/2000/svg\">";
    of << "<g>";
    int kr = 0;
    for( auto& r : myMaze )
    {
        int kc = 0;
        for( auto& c : r )
        {
            if( c.top )
                of << "<line x1=\"" << kc*50
                   << "\" y1=\"" << kr*50
                   <<"\" x2=\"" << (kc+1)*50
                   <<"\" y2=\"" << kr*50
                   <<"\" stroke=\"blue\" stroke-width=\"4\" />";
            if( c.left )
                of << "<line x1=\"" << kc*50
                   << "\" y1=\"" << kr*50
                   <<"\" x2=\"" << kc*50
                   <<"\" y2=\"" << (kr+1)*50
                   <<"\" stroke=\"blue\" stroke-width=\"4\" />";
            if( kc == col_count )
            {
                if( c.right )
                {
                    of << "<line x1=\"" << (kc+1)*50
                       << "\" y1=\"" << kr*50
                       <<"\" x2=\"" << (kc+1)*50
                       <<"\" y2=\"" << (kr+1)*50
                       <<"\" stroke=\"blue\" stroke-width=\"4\" />";
                }
            }
            if( kr == row_count )
            {
                if( c.down )
                {
                    of << "<line x1=\"" << kc*50
                       << "\" y1=\"" << (kr+1)*50
                       <<"\" x2=\"" << (kc+1)*50
                       <<"\" y2=\"" << (kr+1)*50
                       <<"\" stroke=\"blue\" stroke-width=\"4\" />";
                }
            }
            kc++;
        }
        kr++;
    }
    of << "</g>";
    of << "</svg>";
    of.close();
}

void cMaze::solve()
{
    using namespace boost;
    int start = myStartRow * myConfig.ncols;
    typedef adjacency_list<vecS, vecS, directedS> graph_t;
    int N = myConfig.nrows * myConfig.ncols;
    graph_t g( N );
    int kr = 0;
    for( auto& r : myMaze )
    {
        int kc = 0;
        for( auto& c : r )
        {
            if( ! c.top )
            {
                add_edge(
                    kr*myConfig.ncols + kc,
                    (kr-1)*myConfig.ncols + kc,
                    g );
                add_edge(
                    (kr-1)*myConfig.ncols + kc,
                    kr*myConfig.ncols + kc,
                    g );
            }
            if( ( ! c.left ) && kr*myConfig.ncols + kc != start )
            {
                add_edge(
                    kr*myConfig.ncols + kc,
                    kr*myConfig.ncols + kc - 1,
                    g );
                add_edge(
                    kr*myConfig.ncols + kc - 1,
                    kr*myConfig.ncols + kc,
                    g );
            }
            kc++;
        }
        kr++;
    }


    raven::graph::shortestPathInMaze(
        myPath,
        start,
        (myEndRow+1)*myConfig.ncols - 1,
        g );

}

bool cMaze::onPath( int c, int r )
{
    if( ! myPath.size() )
        return false;
    int i = r * myConfig.ncols + c;
    return find( myPath.begin(),myPath.end(),i) != myPath.end();
}

