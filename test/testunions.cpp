//#include "testall.hpp"
#include "../include/extractor/core.hpp"
#include "../include/extractor/instruction.inl"
#include "../include/extractor/modifier.inl"

#include <iostream>
#include <typeinfo>

using namespace extract;

namespace extract
{
    template<>
    struct Union<char, int>
    {
        using join = int;
    };
} // namespace extract

int main()
{

    std::cout << "testing" << std::endl;
    Union<More<One<Extract::blank>, 5>, More<One<Extract::blank>, 5>> u;
    std::cout << typeid(joinbi<More<One<Extract::blank>, 5>, More<One<Extract::blank>, 4>>).name() << std::endl;
    //std::cout << std::is_same<UnionBi<char, char>::join, void>::value << std::endl;
    return 0;
}
