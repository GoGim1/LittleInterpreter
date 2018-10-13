#include <gtest/gtest.h>
//#include "Parser.h"
#include "Lexer.h"

using namespace Lexer;
//using namespace Ast;

TEST(Parser, Build) 
{
    RunLexer("../tests/testcases/LexerCase.txt");
    
}