#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include "checker.hpp"
#include "core.hpp"

#include <type_traits>

namespace extract
{

    class Extractor : public Checker
    {
    public:
        Extractor(const char* text)
            : Checker(text)
        {
        }

        template <Extract Type>
        size_t test_for(int offset = 0) const;

        size_t test_for(TestFor* modifier, int offset = 0) const;

        //template <Extract Type>
        //size_t test_for(size_t from, size_t until) const;

        /**
         * @brief
         *
         * @tparam Modifier - struct which has method
         * with exact signature "public: static size_t test_for(const Extractor*, int)""
         * @param offset offset from current position
         * @return size_t
         */
        template <class Modifier>
        inline auto test_for(int offset = 0) const
            -> typename std::enable_if<
                std::is_same<
                    decltype(Modifier::test_for(this, offset)),
                    size_t>::value,
                size_t>::type
                
        {
            return Modifier::test_for(this, std::forward<int>(offset));
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