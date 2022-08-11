#include "testall.hpp"

using namespace extract;

int main()
{
    const char* text = "0123456789|";
    Extractor   e(text);

    for (size_t i = 0; i < 100; i++) {
        std::cout
            << "One<digit> i=" << i << "\tis digit: "
            << ((e.test_for<One<Extract::digit>>(i) == 1) ? "true" : "false")
            << endl;
    }

    std::cout
        << "One<digit>    "
        << text + e.test_for<One<Extract::digit>>()
        << endl

        << "More<One<digit>, 5>   "
        << text + e.test_for<More<One<Extract::digit>, 5>>()
        << endl

        << "FromTo<One<digit>, 5, 30>   "
        << text + e.test_for<FromTo<One<Extract::digit>, 5, 30>>()
        << endl

        ; // end

    return 0;
}
