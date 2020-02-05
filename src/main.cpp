#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include "cMaze.h"

using namespace std;

std::string Parse( int argc, char *argv[] )
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("input",   po::value<std::string>(), "input file.")
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
        return vm["input"].as<std::string>();
    }
    return "";
}

int main( int argc, char* argv[] )
{
    cMaze theMaze;
    theMaze.read( Parse( argc, argv ) );

    for( auto& s : theMaze.displayText() )
        cout << s << "\n";

    theMaze.displayWindow();

}


