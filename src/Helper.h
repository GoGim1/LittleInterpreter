#pragma once 

#define MakeProgramPtr      make_shared<ProgramNode>
#define MakeStatementPtr    make_shared<StatementNode>
#define MakeSimplePtr       make_shared<SimpleNode>
#define MakeBlockPtr        make_shared<BlockNode>
#define MakeExprPtr         make_shared<ExprNode>
#define MakeFactorPtr       make_shared<FactorNode>
#define MakePrimaryPtr      make_shared<PrimaryNode>
#define MakeTokenPtr        make_shared<Token>


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