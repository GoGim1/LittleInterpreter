#pragma once 

#include <string>
#include "Token.h"

namespace Lexer
{
    using std::string;

    extern Token NextToken();
    extern Token PeekToken();

    extern void RunLexer(string fileName);

};