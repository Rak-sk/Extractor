#ifndef EXTRACTOR_CORE_H
#define EXTRACTOR_CORE_H

#include <cstddef>

namespace extract
{

    class Extractor;

    class Checker;

    class TestFor;

    class Getter;

    /**
     * @brief Variables which can be checked in Extractor
     */
    enum class Extract
    {
        // Checks for space, tabs and new lines
        whitespace,
        // Checks only for spaces and tabs
        blank,
        // 0-9
        digit,
        // a-z and A-Z
        ascii_letter,
        // a-z 
        ascii_small,
        // A-Z
        ascii_capital,
        // .,"'(){}/* etc.
        special,
        // (char)0 - (char)31
        ascii_control,
        ascii,
        // (char)0
        null
    };

    /**
     * @brief Checks if one of the present is in the string
     */
    template <Extract Type>
    struct One;


    /**
     * @brief Checks if any of the present are in the string
     */
    template <Extract Type, Extract...Types>
    struct Any;

    /**
     * @brief Simple implementation of string view for c++11 and c++14
     * 
     */
    struct View
    {
        const char*  first;
        const size_t size;

        inline char operator[](size_t index) const
        {
            return (index < size) ? first[index] : '\0';
        }

        inline const char* begin() const
        {
            return first;
        }

        inline const char* end() const
        {
            return first + size;
        }

    };

} // namespace extract

#endif
