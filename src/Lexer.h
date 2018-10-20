#pragma once 

#include <string>
#include <fstream>
#include <list>
#include <regex>
#include <iterator>
#include <unordered_map>
#include <vector>
#include "Error.h"
#include "Helper.h"
#include "Token.h"

namespace Lexer
{    
    using namespace Util;

    using std::vector;
    using std::fstream;
    using std::list;
    using std::string;

    const Token NextToken();
    const Token& PeekToken();
    
    void RunLexer(string code);
    void RunLexerFromFile(const string& fileName);

};