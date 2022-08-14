#ifndef EXTRACTOR_MATCHER_INL
#define EXTRACTOR_MATCHER_INL

#include "../templutil/typetuple.inl"
#include "modifier.inl"

namespace extract
{

    namespace detail
    {
        using namespace templ::detail;

        template <size_t, class Type>
        struct Run
        {
            static_assert(is_modifier<Type>::value, "Unknown type");

            size_t test_for(const Extractor* extractor, size_t offset)
            {
                return extractor->test_for<Type>(offset);
            }
        };

        template <typename Type>
        struct IsAnd : public std::false_type
        {
        };

        template <typename... Types>
        struct IsAnd<And<Types...>> : public std::true_type
        {
        };

        template <class Type>
        struct DumpAnd
        {
            using type_tuple = typename std::conditional<
                IsAnd<Type>::value,
                typename DumpSafe<Type>::type_tuple::
                    template foreachdump<DumpAnd>,
                Type>::type;
        };

        template <class Type1, class Type2, typename = void>
        struct Unite
        {
            using type_tuple = templ::TypeTuple<Type1, Type2>
        };

        template <class Type1, class Type2>
        struct Unite<Type1, Type2, decltype(check_unionbi<Type1, Type2>())>
        {
            using type_tuple = templ::TypeTuple<joinbi<Type1, Type2>>;
        };

        template <class Tuple, template <size_t, class...> class Consumer = Run, size_t Count = Tuple::count>
        struct Tester : public Tester<Tuple, Consumer, Count - 1>
        {
            static constexpr size_t index = Count - Tuple::count;

            Tester(const char*& const text)
                : Tester<Tuple, Consumer, Count - 1>(text)
            {
            }

            void run()
            {
                size_t temp_result = Consumer<index, typename Tuple::template gettype<index>>::test_for(extractor&, result);
                result += temp_result;
                if (temp_result == 0)
                    found = false;
                else
                    Tester<Tuple, Consumer, Count - 1>::run();
            }
        };

        template <class Tuple, template <size_t, class...> class Consumer>
        struct Tester<Tuple, Consumer, 0>
        {
            Extractor extractor;
            size_t    result = 0;
            bool      found  = true;

            Tester(const char*& const text)
                : extractor(text)
            {
            }

            inline void run()
            {
            }
        };

        template <class... Types>
        struct Pattern
        {
            using pattern = typename templ::TypeTuple<Types...>::
                template foreachdump<DumpAnd>::
                    template foreachdumptwo<Unite>;
        };
    } // namespace detail

} // namespace extract

#endif // EXTRACTOR_MATCHER_INL
