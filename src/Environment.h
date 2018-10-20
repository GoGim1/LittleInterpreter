#pragma once
#include <unordered_map>
#include <string>
#include <variant>

namespace Environment
{
    using std::unordered_map;
    using std::string;
    using std::variant;

    extern unordered_map<string, variant<int, double>> Env;
    
} // Environment
