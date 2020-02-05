#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "wex.h"
#include "cMaze.h"
using namespace std;

void cMaze::generate()
{
    int row_count = 10;
    int col_count = 10;
    /* initialize random seed: */
    srand (time(NULL));
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

void cMaze::read( const std::string& fname )
{
    ifstream in(fname);
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
        while (i < strlen(str1) - 1)
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


vector< string > cMaze::displayText()
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
                s2 << "|   ";
            else
                s2 << "    ";
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

