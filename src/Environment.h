#pragma once
#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <vector>
#include "Error.h"
namespace Environment
{
    using namespace Util;

    using std::unordered_map;
    using std::string;
    using std::variant;
    using std::shared_ptr;
    using std::vector;

    extern unordered_map<string, variant<int, double>>  Env;
    
    extern variant<int, double> EvalDef(const string&, const vector<variant<int, double>>&, int, int);
    extern void                 DefTableClean();
} // Environment
