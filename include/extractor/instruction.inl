#ifndef EXTRACTOR_INSTRUCTION_INL
#define EXTRACTOR_INSTRUCTION_INL

#include "core.hpp"
#include "extractor.hpp"

#pragma region Instructions

namespace extract
{
    template <>
    size_t Extractor::test_for<Extract::whitespace>(size_t offset) const
    {
        switch (get_char(offset)) {
        case ' ':
        case '\n':
        case '\t':
            return true;
        default:
            return false;
        }
    }

    template <>
    size_t Extractor::test_for<Extract::null>(size_t offset) const
    {
        return check_char(offset, '\0');
    }

    template <>
    size_t Extractor::test_for<Extract::blank>(size_t offset) const
    {
        switch (get_char(offset)) {
        case ' ':
        case '\t':
            return true;
        default:
            return false;
        }
    }

    template <>
    size_t Extractor::test_for<Extract::digit>(size_t offset) const
    {
        switch (get_char(offset)) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
        default:
            return false;
        }
    }

    template <>
    size_t Extractor::test_for<Extract::ascii_small>(size_t offset) const
    {
        char c = get_char(offset);
        return 'a' <= c && c <= 'z';
    }

    template <>
    size_t Extractor::test_for<Extract::ascii_capital>(size_t offset) const
    {
        char c = get_char(offset);
        return 'A' <= c && c <= 'Z';
    }

    template <>
    size_t Extractor::test_for<Extract::ascii_letter>(size_t offset) const
    {
        return test_for<Extract::ascii_small>(offset)
            || test_for<Extract::ascii_capital>(offset);
    }

    template <>
    size_t Extractor::test_for<Extract::special>(size_t offset) const
    {
        unsigned char c = get_char(offset);
        return 32 <= c && c <= 47
            || 58 <= c && c <= 64
            || 91 <= c && c <= 96
            || 123 <= c && c <= 126;
    }

    template <>
    size_t Extractor::test_for<Extract::ascii_control>(size_t offset) const
    {
        unsigned char c = get_char(offset);
        return c == 127
            || c <= 31;
    }

    template <>
    size_t Extractor::test_for<Extract::ascii>(size_t offset) const
    {
        unsigned char c = get_char(offset);
        return c <= 127;
    }

} // namespace extract

#pragma endregion Instructions

#include "command.inl"

#pragma region Modifiers

namespace extract
{
    template <Extract Type>
    struct One
    {
        static constexpr Extract mode = Type;

        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            return extractor->test_for<mode>(offset);
        }
    };

    template <Extract Type, Extract... Types>
    struct Any
    {
        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            size_t result = One<Type>::test_for(extractor, offset);
            return result != 0 ? result : Any<Types...>::test_for(extractor, offset);
        }
    };

    template <Extract Type>
    struct Any<Type>
    {
        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            return One<Type>::test_for(extractor, offset);
        }
    };

} // namespace extract

#pragma endregion Modifiers

#endif