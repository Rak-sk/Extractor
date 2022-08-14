#include "testall.hpp"

using namespace extract;

int main()
{
    const char* text = "0123456789|";
    Extractor   e(text);

    // Notice if the text is too small than
    // after reaching the size returns always false
    for (size_t i = 0; i < 35; i++) {
        std::cout
            << "One<digit> i=" << i << "\tis digit: "
            << ((e.test_for<One<Extract::digit>>(i) == 1) ? "true" : "false")
            << endl;
    }

    std::cout
        << endl
        << "Original text: '"
        << text
        << "'"
        << endl

        // Extracts 1 digit if not present than does nothing
        << "One<digit>    "
        << text + e.test_for<One<Extract::digit>>()
        << endl

        // Extracts only 5 digits if not present than does nothing
        << "More<One<digit>, 5>   "
        << text + e.test_for<More<One<Extract::digit>, 5>>()
        << endl

        // Extracts from 5 to 30 digits depending on the text
        << "FromTo<One<digit>, 5, 30>   "
        << text + e.test_for<FromTo<One<Extract::digit>, 5, 30>>()
        << endl

        ; // ! End !
    return 0;
}
