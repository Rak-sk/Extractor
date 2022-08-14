#ifndef EXTRACTOR_CORE_H
#define EXTRACTOR_CORE_H

#include <cstddef>
#include <utility>

namespace extract
{

    class Extractor;

    class Checker;

    class TestFor;

    class Getter;

    template <class Modifier, typename = void>
    class Command;

    /**
     * @brief Variables which can be checked in Extractor
     */
    enum class Extract
    {
        // Checks for space, tabs and new lines
        whitespace,
        // Checks only for spaces and tabs
        blank,
        // 0-9
        digit,
        // a-z and A-Z
        ascii_letter,
        // a-z 
        ascii_small,
        // A-Z
        ascii_capital,
        // .,"'(){}/* etc.
        special,
        // (char)0 - (char)31
        ascii_control,
        ascii,
        // (char)0
        null
    };

    /**
     * @brief Checks if one of the present is in the string
     */
    template <Extract Type>
    struct One;


    /**
     * @brief Checks if any of the present are in the string
     */
    template <Extract Type, Extract... Types>
    struct Any;

    template <class Type1, class Type2, typename = void>
    struct Union;

    template <class Type1, class Type2, typename = void>
    struct UnionBi;

    namespace detail
    {
        template <class Type1, class Type2>
        inline auto check_union()
            -> decltype((void)reinterpret_cast<
                typename Union<Type1,
                    Type2>::join*>((void*)0));

        template <class Type1, class Type2, typename = void>
        struct UnionBi;

        template <class Type>
        struct UnionBi<Type,
            Type,
            decltype(detail::check_union<Type, Type>())>
            : public Union<Type, Type>
        {
        };

        template <class Type1, class Type2>
        struct UnionBi<Type1,
            Type2,
            decltype(detail::check_union<Type1, Type2>())>
            : public Union<Type1, Type2>
        {
        };

        template <class Type1, class Type2, typename>
        struct UnionBi : public Union<Type2, Type1>
        {
        };

        template <class Type1, class Type2>
        inline auto check_unionbi()
            -> decltype((void)reinterpret_cast<
                typename UnionBi<Type1,
                    Type2>::join*>((void*)0));
    
    } // namespace detail

    template <class Type1, class Type2>
    struct UnionBi<Type1,
        Type2,
        decltype(detail::check_unionbi<Type1, Type2>())>
        : public detail::UnionBi<Type1, Type2>
    {
    };

    template <class Type1, class Type2>
    using join = typename Union<Type1, Type2>::join;

    template <class Type1, class Type2>
    using joinbi = typename UnionBi<Type1, Type2>::join;
    
    
    /**
     * @brief Simple implementation of string view for c++11 and c++14
     * 
     */
    struct View
    {
        const char* const first;
        const size_t size;

        inline View& operator=(View view)
        {
            swap(*this, view);
            return *this;
        }

        friend void swap(View& first, View& second) noexcept
        {
            using std::swap;
            swap((const char*&)first.first, (const char*&)second.first);
            swap((size_t&)first.size, (size_t&)second.size);
        }

        inline char operator[](size_t index) const
        {
            return (index < size) ? first[index] : '\0';
        }

        inline const char* begin() const
        {
            return first;
        }

        inline const char* end() const
        {
            return first + size;
        }
    };

} // namespace extract

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#    include <string_view>
namespace extract
{
    using string_view = std::string_view;
}
#else
namespace extract
{
    using string_view = View;
}
#endif

#endif