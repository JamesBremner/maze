#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include "wex.h"
#include "cMaze.h"
using namespace std;

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
        vector< Cell > vCell;

        cell = 1;
        cout << "line " << line << endl;
        int i = 0;
        bool top, down, right, left;
        top = down = right = left = false;
        int j = 0, k = 1;
        cout << "strlen = " << strlen(str1) << endl;
        while (i < strlen(str1) - 1)
        {
            Cell C;
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

