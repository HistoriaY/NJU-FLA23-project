#include <iostream>
#include "TM.hpp"
using namespace std;

static const string usage = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";

int main(int argc, char *argv[])
{
    // parse arguments
    bool verbose = false; // verbose mode
    string tm_path = "";  // file path
    string input = "";    // input string
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == "-h"s || argv[i] == "--help"s)
        {
            cout << usage << endl;
            return 0;
        }
        else if (argv[i] == "-v"s || argv[i] == "--verbose"s)
            verbose = true;
        else if (tm_path.empty())
            tm_path = argv[i];
        else if (input.empty())
            input = argv[i];
        else
        {
            cerr << "wrong parameters: " << argv[i] << endl;
            cerr << usage << endl;
            exit(1);
        }
    }
    if (tm_path.empty())
    {
        cerr << "need a tm file path" << endl;
        exit(1);
    }
    // allow input = ""
    // if (input.empty())
    // {
    //     cerr << "need a input" << endl;
    //     exit(1);
    // }
    TuringMachine tm(tm_path, verbose);
    // parse TM
    tm.parse();
    // simulate TM
    tm.simulate();
    return 0;
}