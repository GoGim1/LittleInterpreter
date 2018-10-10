#pragma once

#include "Ast.h"

namespace Parser
{
    using namespace Ast;

    class Parser
    {
    public:
        PrimaryPtr      ParsePrimary();
        FactorPtr       ParseFactor();
        ExprPtr         ParseExpr();
        BlockPtr        ParseBlock();
        SimplePtr       ParseSimple();
        StatementPtr    ParseStatement();
        ProgramPtr      ParseProgram();
        
    private:
        ProgramPtr      program;

    };

}