#include <iostream>
#include "Parser.h"

using namespace std;
using namespace Parse;

int main(int argc,char *argv[])
{
    if (argc == 1)
    {
        cout << "Please input file path." << endl;
        return 0;
    }
    if (argc > 2) 
    {
        cout << "Only accept one parameter." << endl;
        return 0;
    }
    RunLexerFromFile(argv[1]);
    Parser parser;
    parser.RunParser();
    parser.Eval();
}
