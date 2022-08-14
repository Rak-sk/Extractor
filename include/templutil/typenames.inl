#ifndef TEMPLUTIL_TYPENAMES_INL
#ifdef TEMPLUTIL_DEBUG_MODE

#define TEMPLUTIL_TYPENAMES_INL

#include "typetuple.inl"

#define get_one_type_name(type, name)                        \
    template <>                                              \
    struct TypeName<type>                                    \
    {                                                        \
        constexpr static const char* get() { return name; }; \
    };

#define get_const_ptr_type_names(type, name)                             \
    get_one_type_name(type, name)                                        \
        get_one_type_name(type&, name "&")                               \
            get_one_type_name(const type, name " const")                 \
                get_one_type_name(const type&, name " const&")           \
                    get_one_type_name(const type*, name " const*")       \
                        get_one_type_name(const type*&, name " const*&") \
                            get_one_type_name(type*, name "*")           \
                                get_one_type_name(type*&, name "*&")

#define get_all_type_names(type, name)   \
    get_const_ptr_type_names(type, name) \
        get_const_ptr_type_names(volatile type, "volatile " name)

#define get_all_num_type_names(type) \
    get_all_type_names(type, #type)  \
        get_all_type_names(unsigned type, "unsigned " #type)

namespace templ
{
    namespace detail
    {
        get_all_num_type_names(int);

        get_all_num_type_names(char);

        get_all_num_type_names(short int);

        get_all_num_type_names(long int);

        get_all_num_type_names(long long int);

        get_all_type_names(float, "float");

        get_all_type_names(double, "double");

        get_all_type_names(long double, "long double");

        get_all_type_names(std::string, "std::string");

        get_all_type_names(decltype(std::cout), "std::ostream");

        get_all_type_names(decltype(std::cin), "std::istream");

        template <class... Types>
        struct TypeName<TypeTuple<Types...>>
        {
            static std::string get()
            {
                std::string result = TypeTuple<Types...>::to_string();
                return std::string("TypeTuple<") + result.substr(1, result.size() - 2) + ">";
            };
        };

        template <class Type>
        struct TypeName<TypeName<Type>>
        {
            static std::string get()
            {
                return std::string("TypeName<") + TypeName<Type>::get() + ">";
            };
        };

    } // namespace detail
} // namespace templ

#undef get_one_type_name

#undef get_const_ptr_type_names

#undef get_all_type_names

#undef get_all_num_type_names

#endif
#endif