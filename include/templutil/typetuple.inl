#ifndef TEMPLUTIL_TYPETUPLE_INL
#define TEMPLUTIL_TYPETUPLE_INL

#include <cstddef>
#include <type_traits>

#ifdef TEMPLUTIL_DEBUG_MODE
#    include <iostream>
#    include <typeinfo>
#endif

namespace templ
{

    template <class Type1, class Type2>
    struct Union;

    template <class Type1, class Type2, typename = void>
    struct UnionBi;

    namespace detail
    {
#ifdef TEMPLUTIL_DEBUG_MODE
        template <class Type>
        struct TypeName
        {

            constexpr static const char* get()
            {
                return typeid(Type).name();
            };
        };
#endif

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
            using type = typename GetType<index - 1, Types...>::type;
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

        template <size_t Count, class Type, class... Types>
        struct Split;

#ifdef TEMPLUTIL_DEBUG_MODE
        template <class Type, class... Types>
        struct StringName
        {
            std::string str = std::string(TypeName<Type>::get())
                + ", "
                + StringName<Types...>().str;
        };

        template <class Type>
        struct StringName<Type>
        {
            std::string str = TypeName<Type>::get();
        };
#endif
        template <class... Types>
        class TypeTuple
        {

            template <class TypeTuple>
            struct Concat;

        public:
#ifdef TEMPLUTIL_DEBUG_MODE
            static std::string to_string()
            {
                std::string result = "(";
                result += StringName<Types...>().str;
                return result + ")";
            }
#endif

            static constexpr size_t count = CountTypes<Types...>::count;

            template <size_t index>
            using get_type = typename GetType<index, Types...>::type;

            template <class TypeTuple>
            using concat = typename Concat<TypeTuple>::type_tuple;

            template <class... AddTypes>
            using add = concat<TypeTuple<AddTypes...>>;

            template <size_t count>
            using split = Split<count, Types...>;

            template <size_t index>
            using remove = typename split<index>::new_tuple::template concat<typename split<index + 1>::remaining>;

            template <size_t index, class TypeTuple>
            using insertt = typename split<index>::new_tuple::template concat<TypeTuple>::template concat<typename split<index>::remaining>;

            template <size_t index, class... InsertTypes>
            using insert = insertt<index, TypeTuple<InsertTypes...>>;
        };

        template <template <class...> class Type, class... Types>
        struct Dump<Type<Types...>>
        {
            using type_tuple = TypeTuple<Types...>;
        };

        template <class Type, class... Types>
        struct Split<0, Type, Types...>
        {
            using new_tuple = TypeTuple<>;
            using remaining = TypeTuple<Type, Types...>;
        };

        template <class Type, class... Types>
        struct Split<1, Type, Types...>
        {
            using new_tuple = TypeTuple<Type>;
            using remaining = TypeTuple<Types...>;
        };

        template <class... Types>
        template <class... TupleTypes>
        struct TypeTuple<Types...>::Concat<TypeTuple<TupleTypes...>>
        {
            using type_tuple = TypeTuple<Types..., TupleTypes...>;
        };

        template <size_t count, class Type, class... Types>
        struct Split
        {
            using _smaller  = Split<count - 1, Types...>;
            using _tuple1   = TypeTuple<Type>;
            using _tuple2   = typename _smaller::new_tuple;
            using new_tuple = typename _tuple1::template concat<_tuple2>;
            using remaining = typename _smaller::remaining;
        };

        template <size_t index, class... Types>
        struct Remove
        {
            //using _tuple1    = typename ;
            //using _tuple2    = typename ;
            
        };

        /*template <class... Types>
        struct Split<0, Types...>
        {
            using pack1 = TypeTuple<>;
            using pack2 = TypeTuple<Types...>;
        };

        template <class Type, class... Types>
        struct Split<1, Type, Types...>
        {
            using pack1 = TypeTuple<Type>;
            using pack2 = TypeTuple<Types...>;
        };

        template <size_t Count, class... Types>
        struct Split
        {
            using remove = Split<1, Types...>;
            // using pack1  = remove::pack1::concat<>;
            using pack2  = Dump < ;
        };*/

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

} // namespace templ

#endif