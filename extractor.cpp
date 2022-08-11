
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

#include "include/extractor/extractor.hpp"
#include "include/extractor/testfor.hpp"

size_t extract::Extractor::test_for(TestFor* modifier, size_t offset) const
{
    return modifier->check(this, offset);
}

#pragma endregion Checker
