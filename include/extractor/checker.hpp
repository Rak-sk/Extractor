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
        inline bool check_char(int offset, char character) const
        {
            return (index < -offset || index + offset >= size) ? false : text[index + offset];
        }

        inline void move()
        {
            if (!end())
                ++index;
        }

        inline void move(size_t steps)
        {
            steps += index;
            index = steps < size ? steps : size;
        }

        inline bool end() const
        {
            return index == size;
        }

        inline char get_char() const
        {
            return text[index];
        }

        /**
         * @brief Gets character from string. If character is not present
         * returns null ('\0').
         * 
         * @param offset added to current position
         * @return char 
         */
        inline char get_char(int offset) const
        {
            return (index < -offset || index + offset >= size) ? '\0' : text[index + offset];
        }

        void get_chars(char array[], size_t size) const;

        size_t length() const
        {
            return size;
        }

        size_t remaining() const
        {
            return size - index;
        }

    protected:
        const char* const text;
        size_t            index;
        const size_t      size;
    };
} // namespace extract

#endif