#include <gtest/gtest.h>
#include "Lexer.h"

using namespace Lexer;
// var i = 2;
// if (i == 2.0)
// {
//   i = 3;
// }
TEST(Lexer, Basic) 
{
    RunLexerFromFile("../tests/testcases/LexerCase.txt");
    ASSERT_EQ(PeekToken().getPosX(), 0);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), "var");
    ASSERT_EQ(NextToken().getType(), Token::Type::VAR);
    
    ASSERT_EQ(PeekToken().getPosX(), 4);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), "i");
    ASSERT_EQ(NextToken().getType(), Token::Type::IDENTIFIER);

    ASSERT_EQ(PeekToken().getPosX(), 6);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), "=");
    ASSERT_EQ(NextToken().getType(), Token::Type::ASSIGN);


    ASSERT_EQ(PeekToken().getPosX(), 8);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), "2");
    ASSERT_EQ(NextToken().getType(), Token::Type::INTEGER);

    ASSERT_EQ(PeekToken().getPosX(), 9);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), ";");
    ASSERT_EQ(NextToken().getType(), Token::Type::SEMICOLON);

    ASSERT_EQ(PeekToken().getPosX(), 0);
    ASSERT_EQ(PeekToken().getPosY(), 1);
    ASSERT_EQ(PeekToken().getValue(), "if");
    ASSERT_EQ(NextToken().getType(), Token::Type::IF);
    
    ASSERT_EQ(PeekToken().getPosX(), 3);
    ASSERT_EQ(PeekToken().getPosY(), 1);
    ASSERT_EQ(PeekToken().getValue(), "(");
    ASSERT_EQ(NextToken().getType(), Token::Type::LBRACKET);

    ASSERT_EQ(PeekToken().getPosX(), 4);
    ASSERT_EQ(PeekToken().getPosY(), 1);
    ASSERT_EQ(PeekToken().getValue(), "i");
    ASSERT_EQ(NextToken().getType(), Token::Type::IDENTIFIER);
    
    ASSERT_EQ(PeekToken().getPosX(), 6);
    ASSERT_EQ(PeekToken().getPosY(), 1);
    ASSERT_EQ(PeekToken().getValue(), "==");
    ASSERT_EQ(NextToken().getType(), Token::Type::EQUAL);
    
    ASSERT_EQ(NextToken().getType(), Token::Type::FLOAT);

    ASSERT_EQ(NextToken().getType(), Token::Type::RBRACKET);
    
    ASSERT_EQ(PeekToken().getPosX(), 0);
    ASSERT_EQ(PeekToken().getPosY(), 2);
    ASSERT_EQ(PeekToken().getValue(), "{");
    ASSERT_EQ(NextToken().getType(), Token::Type::LBRACE);
    
    ASSERT_EQ(NextToken().getType(), Token::Type::IDENTIFIER);
    
    ASSERT_EQ(NextToken().getType(), Token::Type::ASSIGN);
    
    ASSERT_EQ(PeekToken().getPosX(), 6);
    ASSERT_EQ(PeekToken().getPosY(), 3);
    ASSERT_EQ(PeekToken().getValue(), "3");
    ASSERT_EQ(NextToken().getType(), Token::Type::INTEGER);
    
    ASSERT_EQ(NextToken().getType(), Token::Type::SEMICOLON);
    
    ASSERT_EQ(NextToken().getType(), Token::Type::RBRACE);
    
    ASSERT_EQ(PeekToken().getPosX(), 1);
    ASSERT_EQ(PeekToken().getPosY(), 4);
    ASSERT_EQ(PeekToken().getValue(), "EOF");
    ASSERT_EQ(NextToken().getType(), Token::Type::_EOF);

}

TEST(Lexer, Error) 
{
    HandleTry
        RunLexer(R"( \ ~
`   ~)");
    HandleCatch
        ASSERT_EQ(DumpError(), R"TEST([Line 0, 1]: Unknown token: "\".
[Line 0, 3]: Unknown token: "~".
[Line 1, 0]: Unknown token: "`".
[Line 1, 4]: Unknown token: "~".
)TEST");
    HandleEnd
}

TEST(Lexer, Func) 
{
    RunLexer(R"(defuse def, )");
    ASSERT_EQ(PeekToken().getPosX(), 0);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), "defuse");
    ASSERT_EQ(NextToken().getType(), Token::IDENTIFIER);

    ASSERT_EQ(PeekToken().getPosX(), 7);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), "def");
    ASSERT_EQ(NextToken().getType(), Token::DEF);

    ASSERT_EQ(PeekToken().getPosX(), 10);
    ASSERT_EQ(PeekToken().getPosY(), 0);
    ASSERT_EQ(PeekToken().getValue(), ",");
    ASSERT_EQ(NextToken().getType(), Token::COMMA);
    
}