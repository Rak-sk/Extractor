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

        size_t test_for(TestFor* checker, size_t offset = 0) const;

        size_t get_chars(Getter* getter, size_t offset = 0) const;

        size_t get_chars(TestFor* checker, string_view& view, size_t offset = 0) const;

        bool get_chars_move(Getter* getter)
        {
            size_t size = get_chars(getter);
            index += size;
            return size;
        }

        bool get_chars_move(TestFor* checker, string_view& view)
        {
            size_t size = get_chars(checker, view);
            index += size;
            return size;
        }

        template <Extract Type>
        size_t test_for(size_t offset = 0) const;

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
         * @brief Test if special pattern or variable is in teh text.
         *
         * @tparam Modifier type which has method
         * with exact signature "public: static size_t 
         * test_for(const Extractor*, size_t)""
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

        template <class Modifier>
        auto get_chars(string_view& view, size_t offset = 0) const
            -> typename std::enable_if<
                std::is_same<
                    decltype(test_for<Modifier>(offset)),
                    size_t>::value,
                size_t>::type
        {
            size_t size = test_for<Modifier>();
            if (size != 0)
                view = string_view { text + index, size };
            return size;
        }

        template <class Modifier>
        auto get_chars_move(string_view& view)
            -> typename std::enable_if<
                std::is_same<
                    decltype(get_chars<Modifier>(view)),
                    size_t>::value,
                bool>::type
        {
            size_t size = get_chars<Modifier>(view);
            index += size;
            return size;
        }

        template <class Getter>
        inline auto get_chars(Getter& getter, size_t offset = 0) const
            -> typename std::enable_if<
                std::is_same<
                    decltype(Getter().get_chars(
                        (const Extractor*)this,
                        offset)),
                    size_t>::value,
                size_t>::type

        {
            return getter.get_chars(this, offset);
        }

        template <class Getter>
        inline auto get_chars_move(Getter& getter)
            -> typename std::enable_if<
                std::is_same<
                    decltype(get_chars(getter)),
                    size_t>::value,
                bool>::type

        {
            size_t size = get_chars(getter);
            index += size;
            return size;
        }

        template <Extract Type>
        bool get_chars(string_view& view, size_t offset = 0) const
        {
            size_t size = test_for<Type>(offset);
            if (size != 0)
                view = string_view { text + index, size };
            return size;
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
        bool get_chars_move(string_view& view)
        {
            size_t size = get_chars<Type>(view);
            index += size;
            return size;
        }
    };
} // namespace extract

#endif