#define TEMPLUTIL_DEBUG_MODE
#include "../include/templutil/typenames.inl"

using namespace templ::detail;

auto endl = '\n';

template <size_t, class Type>
struct Consumer
{
    static void call()
    {
        std::cout << TypeName<Type>::get() << endl;
    };
};

int main(int argc, char const* argv[])
{
    std::cout
        << TypeName<templ::TypeTuple<char, short, int, long>::insert<3, float, double>>::get()
        << endl

        << TypeName<templ::TypeTuple<float*, templ::TypeTuple<char, templ::TypeTuple<templ::TypeTuple<int>>, long>, templ::TypeTuple<const char, const int>, double*>::dumpallrec<>>::get()
        << endl

        ; // ! End !

    templ::TypeTuple<char, short, int, long>::foreach<Consumer>();
    return 0;
}
