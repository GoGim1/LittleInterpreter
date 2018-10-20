#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

class NumberWraper {
public:
    NumberWraper() {}
    
    template<class NumType>
    NumberWraper(NumType t): _s(to_string(t)) {
    }
    
    template<class NumType>
    operator NumType() const 
    {
        return boost::lexical_cast<NumType>(_s);
    }

    template<class NumType>
    NumberWraper& operator=(const NumType& i)
    {
        this->_s = to_string(i);
        return *this;
    }

    template<class NumType>
    NumType operator+(const NumType& i)
    {
        return NumType(*this) + i;
    }

private:
    string _s;
};


template<class NumType>
NumType operator+(const NumType& lhs, NumberWraper& rhs)
{
    return rhs.operator+<NumType>(lhs);
}   


struct NumberTrait {}

