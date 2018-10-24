#include "Environment.h"
#include "Ast.h"

namespace Environment
{
    using namespace Ast;

    unordered_map<string, variant<int, double>>     Env;
    unordered_map<string, DefPtr>                   defTable;

    void DefiniteFunc(const DefPtr& pDefRhs)
    {
        string index = pDefRhs->GetFunctionName();
        defTable[index] = pDefRhs;
    }
    
    variant<int, double> EvalDef(const string& funName, const vector<variant<int, double>>& args, int posX, int posY)
    {
        if (defTable.find(funName) == defTable.end())
            throw Error("Runtime error: undefined function \"" + funName + "\".", posX, posY);
        auto pDef = defTable[funName];
        return pDef->Eval(args, posX, posY);
    }

    void DefTableClean()
    {
        defTable.clear();
    }
} // Environment
