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
        size_t test_for(size_t offset = 0) const;

        size_t test_for(TestFor* modifier, size_t offset = 0) const;

        /**
         * @brief Test if special pattern or variable is in teh text.
         *
         * @tparam Modifier - struct which has method
         * with exact signature "public: static size_t test_for(const Extractor*, int)""
         * @param offset offset from current position
         * @return size_t
         */
        template <class Modifier>
        inline auto test_for(size_t offset = 0) const
            -> typename std::enable_if<
                std::is_same<
                    decltype(Modifier::test_for(this, offset)),
                    size_t>::value,
                size_t>::type

        {
            return Modifier::test_for(this, offset);
        }

        /**
         * @brief Checks if character is present at current position.
         *
         * @tparam Character chracter to check for
         * @return true
         * @return false
         */
        template <char Character>
        inline bool test_for() const
        {
            return Character == get_char();
        }

        /**
         * @brief Checks if any of the present can be found at current position.
         *
         * @tparam Characters chracters to check for
         * @return true
         * @return false
         */
        template <char Character, char... Characters>
        inline bool test_for() const
        {
            return test_for<Character>() && test_for<Characters...>();
        }

        /**
         * @brief Gets a character or charcters from text
         * specified by the extract type and moves.
         *
         * @tparam Type
         * @param view reference where the result should be stored
         * @return true
         * @return false
         */
        template <Extract Type>
        bool get_chars(View& view)
        {
            size_t size = test_for<Type>();
            if (size == 0)
                return false;
            view = View { text + index, size };
            index += size;
            return true;
        }
    };
} // namespace extract

#endif