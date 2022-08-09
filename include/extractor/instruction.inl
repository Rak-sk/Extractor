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

} // namespace extract
