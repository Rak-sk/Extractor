#ifndef EXTRACTOR_CORE_H
#define EXTRACTOR_CORE_H

#include <cstddef>

namespace extract
{

    class Extractor;

    class Checker;

    class TestFor;

    class Getter;

    template <class INTR>
    class Command;

    enum class Extract
    {
        whitespace,
        blank,
        digit,
        ascii_letter,
        ascii_small,
        ascii_capital,
        special,
        ascii_control,
        ascii
    };

    template <Extract Type>
    struct One
    {
        using type = Type;
    };

    template <Extract Type>
    struct Any;

    template <Extract Type>
    struct Plus;

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