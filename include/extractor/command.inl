#include "core.hpp"


namespace extract
{

    template <class INST>
    class Command : public TestFor
    {
    public:
        bool check(Extractor* extractor) override
        {
        }

        ~Command() override
        {
        }
    };

} // namespace extractor
