
#pragma region Checker

#include "include/extractor/checker.hpp"

#include <cstring>
#include <stdexcept>

extract::Checker::Checker(const char* text)
    : text(text)
    , index(0)
    , size(strlen(text))
{
}

void extract::Checker::get_chars(char array[], size_t size) const
{
    if (size > this->size - index)
        throw std::out_of_range("String out of range");
    strncat(array, text + index, size);
}

#pragma endregion Checker

#pragma region Checker

#include "include/extractor/testfor.hpp"
#include "include/extractor/extractor.hpp"

size_t extract::Extractor::test_for(TestFor* modifier, int offset) const
{
    return modifier->check(this, offset);
}

#pragma endregion Checker


using namespace extract;

#include <iostream>



#include "include/extractor/extractor.hpp"

#include "include/extractor/command.inl"

#include "include/extractor/instruction.inl"

#include "include/extractor/modifier.inl"

struct t 
{
    //extract::Extract::ascii;

    static size_t test_for(const Extractor*, int)
    {
    }
};


int main(int argc, char const *argv[])
{
    Extractor e("fffff");
    auto temp = Command<One<Extract::ascii>>();
    //auto x = modifier_checker<t>();
    auto x = More<t, 5>();
    //std::cout << e.test_for<FromTo<t, 5, 6>>();
    return 0;
}

