#ifndef EXTRACOTR_COMMAND_INL
#define EXTRACOTR_COMMAND_INL

#include "core.hpp"
#include "extractor.hpp"
#include "testfor.hpp"

namespace extract
{

    template <class Modifier>
    class Command<Modifier,
        typename std::enable_if<
            std::is_same<
                decltype(Extractor("").test_for<Modifier>()), size_t>::value>::type>
        : public TestFor
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

        inline size_t get_chars(const Extractor* extractor, size_t offset)
        {
            return extractor->get_chars<Modifier>(view, offset);
        }

        ~Command() override = default;

        string_view view;
    };

    

} // namespace extract

#endif