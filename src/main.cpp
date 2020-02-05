#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include "cMaze.h"

using namespace std;

class cConfig
{
public:

    std::string inputfilename;
    bool fgenerate;

    cConfig()
    : inputfilename("")
    , fgenerate( false )
    {

    }

    void Parse( int argc, char *argv[] );
};

void cConfig::Parse( int argc, char *argv[] )
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("input",   po::value<std::string>(), "input file.")
    ("gen",     po::bool_switch( &fgenerate ),"Generate new maze")
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

}

int main( int argc, char* argv[] )
{
    cMaze theMaze;
    cConfig theConfig;
    theConfig.Parse( argc, argv );

    if( ! theConfig.fgenerate )
        theMaze.read( theConfig.inputfilename );
    else
        theMaze.generate();

    for( auto& s : theMaze.displayText() )
        cout << s << "\n";

    //theMaze.displayWindow();

}


