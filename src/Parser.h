#pragma once
#include <vector>
#include "Error.h"
#include "Ast.h"

namespace Parse
{
    using namespace Ast;
    using namespace Util;

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
        
        void            RunParser()     {  program = ParseProgram(); } 
        string          Dump()          {  return program->Dump(); }
        void            HandleError();
        void            AddError(const Error&);

        // (DEBUG)
        void            DumpErrorList();
    private:
        ProgramPtr      program;
        vector<Error>   errorList;
    };

}