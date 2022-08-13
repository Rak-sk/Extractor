#ifndef EXTRACTOR_CORE_H
#define EXTRACTOR_CORE_H

#include <cstddef>

namespace extract
{

    class Extractor;

    class Checker;

    class TestFor;

    class Getter;

    template <class Modifier, typename = void>
    class Command;

    enum class Extract
    {
        whitespace,
        blank,
        digit,
        ascii_letter,
        ascii_small,
        ascii_capital,
        special,
        ascii_control,
        ascii,
        null
    };

    template <Extract Type>
    struct One;

    template <Extract Type, Extract... Types>
    struct Any;

    template <class Type1, class Type2>
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
    
    
    struct View
    {
        const char*  first;
        const size_t size;

        inline void operator=(const View& view)
        {
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