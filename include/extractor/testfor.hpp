#include "core.hpp"

namespace extract
{
    class TestFor
    {
    public:
        virtual size_t check(const Extractor* extractor) = 0;
        ~TestFor()                                       = delete;
    };

} // namespace extract