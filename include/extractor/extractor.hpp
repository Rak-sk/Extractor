#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "checker.hpp"
#include "core.hpp"

namespace extract
{

    class Extractor : public Checker
    {
    public:
        template <typename>
        friend class Command;

        Extractor(const char* text);

        template <Extract Type, class... Ts>
        size_t test_for(Ts... args) const;

        template <class Type, class... Ts>
        size_t test_for(Ts... args) const
        {
            return test_for<Type::type, Ts...>(std::forward<Ts>(args)...);
        }

        template <char Character>
        inline bool test_for() const
        {
            return Character == get_char();
        }

        template <char Character, char... Characters>
        inline bool test_for() const
        {
            return test_for<Character>() && test_for<Characters...>();
        }

        template <Extract Type, class... Ts>
        bool get_chars(View* view, Ts... args)
        {
            size_t size = test_for<Type, Ts...>(std::forward<Ts>(args)...);
            if (size == 0)
                return false;
            *view = View { text + index, size };
            index += size;
            return true;
        }

    };
} // namespace extract

#endif