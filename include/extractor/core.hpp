#ifndef EXTRACTOR_CORE_H
#define EXTRACTOR_CORE_H

#include <cstddef>

namespace extract
{

    class Extractor;

    class Checker;

    class TestFor;

    class Getter;

    template<class Modifier, typename = void>
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
        ascii,
        null
    };

    template <Extract Type>
    struct One;

    template <Extract Type, Extract...Types>
    struct Any;

    struct View
    {
        const char*  first;
        const size_t size;

        inline void operator=(const View& view)
        {
        }

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

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#    include <string_view>
namespace extract
{
    using string_view = std::string_view;
}
#else
namespace extract
{
    using string_view = View;
}
#endif

#endif