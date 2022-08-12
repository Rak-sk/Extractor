#define TEMPLUTIL_DEBUG_MODE
#include "../include/templutil/typenames.inl"

using namespace templ::detail;

int main(int argc, char const *argv[])
{
    std::cout << TypeName<TypeTuple<char, short, int, long>::insert<3, float, double>>::get();
    return 0;
}
