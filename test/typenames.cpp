#define TEMPLUTIL_DEBUG_MODE
#include "../include/templutil/typenames.inl"

using namespace templ::detail;

auto endl = '\n';

int main(int argc, char const* argv[])
{
    std::cout
        << TypeName<TypeTuple<char, short, int, long>::insert<3, float, double>>::get()
        << endl

        << TypeName<TypeTuple<float*, TypeTuple<char, TypeTuple<TypeTuple<int>>, long>, TypeTuple<const char, const int>, double*>::dumpallrec>::get()
        << endl

        ; // ! End !
    return 0;
}
