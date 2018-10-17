#pragma once 

#include <memory>
#define MakeProgramPtr      std::make_shared<ProgramNode>
#define MakeStatementPtr    std::make_shared<StatementNode>
#define MakeSimplePtr       std::make_shared<SimpleNode>
#define MakeBlockPtr        std::make_shared<BlockNode>
#define MakeExprPtr         std::make_shared<ExprNode>
#define MakeFactorPtr       std::make_shared<FactorNode>
#define MakePrimaryPtr      std::make_shared<PrimaryNode>
#define MakeTokenPtr        std::make_shared<Token>


#ifdef DEBUG
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;
#define Print(str)          cout << (str) << endl
#define Assert(a, b)        assert((a) && (b))

#define HandleTry           \
try                         \
{                               
#define HandleCatch         \
}                           \
catch (Error& e)            \
{
#define HandleEnd           \
}
#else 
#define Print(str)  
#define Assert(a, b)        
#endif