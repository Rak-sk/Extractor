#ifndef EXTRACOTR_TESTFOR_H
#define EXTRACOTR_TESTFOR_H

#include "core.hpp"

namespace extract
{
    class TestFor
    {
    public:
        friend Extractor;

    protected:
        virtual size_t check(const Extractor*, size_t offset) = 0;

    public:
        virtual ~TestFor() = default;
    };

    size_t Extractor::test_for(TestFor* checker, size_t offset) const
    {
        return checker->check(this, offset);
    }

    size_t Extractor::get_chars(TestFor* checker, string_view& view, size_t offset) const
    {
        size_t size = checker->check(this, offset);
        view = string_view{text + offset, size};
        return size;
    }

} // namespace extract

#endif