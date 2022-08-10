#include "core.hpp"
#include "extractor.hpp"

namespace extract
{
    template <>
    size_t Extractor::test_for<Extract::whitespace>() const
    {
        switch (get_char()) {
        case ' ':
        case '\n':
        case '\t':
            return true;
        default:
            return false;
        }
    }

    template <>
    size_t Extractor::test_for<Extract::blank>() const
    {
        switch (get_char()) {
        case ' ':
        case '\t':
            return true;
        default:
            return false;
        }
    }

    template <>
    size_t Extractor::test_for<Extract::digit>() const
    {
        switch (get_char()) {
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
    size_t Extractor::test_for<Extract::ascii_small>() const
    {
        char c = get_char();
        return 'a' <= c && c <= 'z';
    }

    template <>
    size_t Extractor::test_for<Extract::ascii_capital>() const
    {
        char c = get_char();
        return 'A' <= c && c <= 'Z';
    }

    template <>
    size_t Extractor::test_for<Extract::ascii_letter>() const
    {
        return test_for<Extract::ascii_small>()
            || test_for<Extract::ascii_capital>();
    }

    template <>
    size_t Extractor::test_for<Extract::special>() const
    {
        unsigned char c = get_char();
        return 32 <= c && c <= 47
            || 58 <= c && c <= 64
            || 91 <= c && c <= 96
            || 123 <= c && c <= 126;
    }

    template <>
    size_t Extractor::test_for<Extract::ascii_control>() const
    {
        unsigned char c = get_char();
        return c == 127
            || c <= 31;
    }

    template <>
    size_t Extractor::test_for<Extract::ascii>() const
    {
        unsigned char c = get_char();
        return c <= 127;
    }

} // namespace extract
