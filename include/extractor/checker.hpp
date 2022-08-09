#ifndef EXTRACTOR_CHECKER_H
#define EXTRACTOR_CHECKER_H

#include "core.hpp"

namespace extract
{
    class Checker
    {
    public:
        Checker(const char* text);

        inline bool check_char(char character) const
        {
            return character == get_char();
        }

        /**
         * @brief Checks if character is in string without throwing exeption.
         * If character is outside the string returned value is false.
         *
         * @param index offset of current position
         * @param character character to be checked
         * @return true
         * @return false
         */
        bool check_char(int index, char character) const;

        inline void move()
        {
            if (!end())
                ++index;
        }

        inline bool end() const
        {
            return index == size;
        }

        inline char get_char() const
        {
            return text[index];
        }

        void get_chars(char array[], size_t size) const;

    protected:
        const char* const text;
        size_t            index;
        const size_t      size;
    };
} // namespace extract

#endif