#ifndef TEMPLUTIL_TYPETUPLE_INL
#define TEMPLUTIL_TYPETUPLE_INL

#define TEMPLUTIL_DEBUG_MODE

#include <cstddef>
#include <type_traits>

#ifdef TEMPLUTIL_DEBUG_MODE
#    include <iostream>
#    include <typeinfo>
#endif

namespace extract
{

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

        template <class Type>
        struct Dump;

        template <size_t index, class Type, class... Types>
        struct GetType
        {
            using type = GetType<index - 1, Types...>::type;
        };

        template <class Type, class... Types>
        struct GetType<0, Type, Types...>
        {
            using type = Type;
        };

        template <class... Types>
        struct CountTypes;

        template <>
        struct CountTypes<>
        {
            static constexpr size_t count = 0;
        };

        template <class Type>
        struct CountTypes<Type>
        {
            static constexpr size_t count = 1;
        };

        template <class Type, class... Types>
        struct CountTypes<Type, Types...>
        {
            static constexpr size_t count = 1 + CountTypes<Types...>::count;
        };

        template <size_t Count, class... Types>
        struct Split;

        template <class Type>
        const char* get_type_name()
        {
#ifdef TEMPLUTIL_DEBUG_MODE
            return typeid(Type).name();
#else
            return "";
#endif
        };
#ifdef TEMPLUTIL_DEBUG_MODE
        template<class Type, class... Types>
        struct StringName
        {
            static std::string str = std::string(get_type_name<Type>()) + ", " + StringName<Types...>::str;
        };

        template<class Type>
        struct StringName
        {
            static std::string = get_type_name<Type>();
        };
#endif
        template <class... Types>
        struct TypeTuple
        {

#ifdef TEMPLUTIL_DEBUG_MODE    
            static std::string to_string()
            {
                std::string result = "(";
                result += StringName<Types...>::str;
                return result + ")";
            }
#endif

            template <class TypeTuple>
            struct Concentrate;

            template <>
            struct Concentrate<TypeTuple<>>
            {
                using type_tuple = TypeTuple<Types...>;
            };

            template <class... TupleTypes>
            struct Concentrate<TypeTuple<TupleTypes...>>
            {
                using type_tuple = TypeTuple<Types..., TupleTypes...>;
            };

            static constexpr size_t count = CountTypes<Types...>::count;

            template <size_t index>
            using get_type = typename GetType<index, Types...>::type;

            template <class TypeTuple>
            using concat = typename Concentrate<TypeTuple>::type_tuple;


        };

        template <class... Types>
        struct Split<0, Types...>
        {
            using pack1 = TypeTuple<>;
            using pack2 = TypeTuple<Types...>;
        };

        template <size_t Count, class Type, class... Types>
        struct Split<Count, Type, Types...>
        {
            static_assert(Count <= CountTypes<Types...>::count, "Out of range");

            using next_split = Split<Count - 1, Types...>;
            using pack1      = TypeTuple<Type>::concat<
                next_split::pack1>;
            using pack2 = next_split;
        };

        template <>
        struct TypeTuple<>
        {
#ifdef TEMPLUTIL_DEBUG_MODE
            static std::string to_string()
            {
                return "()";
            }
#endif

            template <class TypeTuple>
            using concat = TypeTuple;
        };

        template <template <class...> class Type, class... Types>
        struct Dump<Type<Types...>>
        {
            using type_tuple = TypeTuple<Types...>;
        };

    }; // namespace detail

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

} // namespace extract

#endif