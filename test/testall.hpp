#ifndef EXTRACTOR_TEST_ALL_HPP
#define EXTRACTOR_TEST_ALL_HPP

#include <iostream>

#include "../include/extractor/instruction.inl"
#include "../include/extractor/modifier.inl"
#include "../include/extractor/command.inl"
#include "../extractor.cpp"

static constexpr auto endl = '\n';

decltype(std::cout)& operator<<(decltype(std::cout)& out, extract::View view)
{
    for (char c : view)
        out << c;
    return out;
}
    
#endif  