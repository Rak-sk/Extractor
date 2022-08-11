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

} // namespace extract

#endif