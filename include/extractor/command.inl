#ifndef EXTRACOTR_COMMAND_INL
#define EXTRACOTR_COMMAND_INL

#include "core.hpp"
#include "extractor.hpp"
#include "testfor.hpp"

namespace extract
{

    template <class Modifier, std::enable_if_t<std::is_same<decltype(Extractor("").test_for<Modifier>(int())), size_t>::value, bool> = true>
    class Command : public TestFor
    {

    protected:
        virtual size_t check(const Extractor* extractor, size_t offset) override
        {
            return Modifier::test_for(extractor, offset);
        }

    public:
        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            return Modifier::test_for(extractor, offset);
        }

        ~Command() override = default;
    };

} // namespace extract

#endif