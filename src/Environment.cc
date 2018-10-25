#include "Environment.h"
#include "Ast.h"

namespace Environment
{
    using namespace Ast;

    unordered_map<string, variant<int, double>>     Env;
    unordered_map<string, DefPtr>                   defTable;

    static int depth = 0;

    void DefiniteFunc(const DefPtr& pDefRhs)
    {
        string index = pDefRhs->GetFunctionName();
        defTable[index] = pDefRhs;
    }
    
    variant<int, double> EvalDef(const string& funName, const vector<variant<int, double>>& args, int posX, int posY)
    {
        depth++;
        if (depth == 50)
        {
            depth = 0;
            throw Error("Runtime error: the depth of stack is out of range.", posX, posY);
        }
        if (defTable.find(funName) == defTable.end())
        {
            depth = 0;
            throw Error("Runtime error: undefined function \"" + funName + "\".", posX, posY);
        }
        auto pDef = defTable[funName];
        try
        {
            auto ret = pDef->Eval(args, posX, posY);
            depth--;
            return ret;
        }
        catch (Error& e)
        {
            depth = 0;
            throw;
        }
        
    }

    void DefTableClean()
    {
        defTable.clear();
    }
} // Environment
