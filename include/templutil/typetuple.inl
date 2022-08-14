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

        template <class Type>
        struct Dump;

        template <class Type, typename = void>
        struct DumpSafe;

        template <class Type, typename = void>
        struct DumpSafeRecursive;

        template <class Tuple, template <class...> class Dumper = DumpSafe, size_t Last = Tuple::count - 1>
        struct DumpAll;

        template <class... Types>
        struct CountTypes;

        template <size_t Count, class Type, class... Types>
        struct Split;

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
    }; // namespace detail

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
            result += detail::StringName<Types...>().str;
            return result + ")";
        }
#endif

        static constexpr size_t count = detail::CountTypes<Types...>::count;

        template <size_t index>
        using gettype = typename detail::GetType<index, Types...>::type;

        template <class TypeTuple>
        using concat = typename Concat<TypeTuple>::type_tuple;

        template <class... AddTypes>
        using add = concat<TypeTuple<AddTypes...>>;

        template <size_t count>
        using split = detail::Split<count, Types...>;

        template <size_t index>
        using remove = typename split<index>::new_tuple::
            template concat<
                typename split<index + 1>::remaining>;

        template <size_t index, class TypeTuple>
        using insertt = typename split<index>::new_tuple::
            template concat<TypeTuple>::
                template concat<
                    typename split<index>::remaining>;

        template <size_t index, class... InsertTypes>
        using insert = insertt<index, TypeTuple<InsertTypes...>>;

        template <size_t index, template <class...> class Dumper = detail::Dump>
        using dump = typename split<index>::new_tuple::
            template concat<
                typename Dumper<
                    gettype<index>>::type_tuple>::
                template concat<
                    typename split<index + 1>::remaining>;

        template <size_t index>
        using dumpsafe = dump<index, detail::DumpSafe>;

        template <size_t index>
        using dumprec = dump<index, detail::DumpSafeRecursive>;

        template <template <class...> class Dumper>
        using foreachdump = typename detail::DumpAll<
            TypeTuple<Types...>,
            Dumper>::new_tuple;

        template <typename = void>
        using dumpall = foreachdump<detail::DumpSafe>;

        template <typename = void>
        using dumpallrec = foreachdump<detail::DumpSafeRecursive>;
    };

    template <class... Types>
    template <class... TupleTypes>
    struct TypeTuple<Types...>::Concat<TypeTuple<TupleTypes...>>
    {
        using type_tuple = TypeTuple<Types..., TupleTypes...>;
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

    namespace detail
    {

        template <template <class...> class Type, class... Types>
        struct Dump<Type<Types...>>
        {
            template <class... TTypes>
            using type       = Type<TTypes...>;
            using type_tuple = TypeTuple<Types...>;
        };

        template <class Type>
        auto check_dump()
            -> decltype((void)reinterpret_cast<
                typename Dump<Type>::type_tuple*>(
                (void*)0));

        template <class Type>
        struct DumpSafe<Type,
            decltype(check_dump<Type>())>
            : public Dump<Type>
        {
        };

        template <class Type, typename>
        struct DumpSafe
        {
            using type_tuple = TypeTuple<Type>;
            using type       = Type;
        };

        template <class Type>
        struct DumpSafeRecursive<Type,
            decltype(check_dump<Type>())>
        {
            using type_tuple = typename Dump<
                Type>::type_tuple::template foreachdump<DumpSafeRecursive>;
        };

        template <class Type, typename>
        struct DumpSafeRecursive : public DumpSafe<Type>
        {
        };

        template <class Tuple, template <class...> class Dumper>
        struct DumpAll<Tuple, Dumper, 0>
        {
            using new_tuple = typename DumpSafe<
                typename Tuple::template gettype<0>>::type_tuple;
        };

        template <class Tuple, template <class...> class Dumper, size_t Last>
        struct DumpAll
        {
            using new_tuple = typename DumpAll<Tuple,
                Dumper,
                Last - 1>::
                new_tuple::template concat<
                    typename Dumper<
                        typename Tuple::template gettype<
                            Last>>::type_tuple>;
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

        template <size_t count, class Type, class... Types>
        struct Split
        {
            using _smaller  = Split<count - 1, Types...>;
            using _tuple1   = TypeTuple<Type>;
            using _tuple2   = typename _smaller::new_tuple;
            using new_tuple = typename _tuple1::template concat<_tuple2>;
            using remaining = typename _smaller::remaining;
        };

    }; // namespace detail

} // namespace templ

#endif