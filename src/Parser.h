#pragma once

#include "Ast.h"

namespace Parse
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
        
        void            RunParser() {  program = ParseProgram(); } 
        string          Dump() {  return program->Dump(); }
    private:
        ProgramPtr      program;

    };

}