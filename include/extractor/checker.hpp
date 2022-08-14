#ifndef EXTRACTOR_CHECKER_H
#define EXTRACTOR_CHECKER_H

#include "core.hpp"

#include <cstring>
#include <stdexcept>

namespace extract
{
    class Checker
    {
    public:
        Checker(const char* text)
            : text(text)
            , index(0)
            , size(strlen(text))
        {
        }

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
        inline bool check_char(size_t offset, char character) const
        {
            offset += index;
            return offset >= size ? false : text[offset];
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
        inline char get_char(size_t offset) const
        {
            offset += index;
            return offset >= size ? '\0' : text[offset];
        }

        void get_chars(char array[], size_t size) const
        {
            if (size > this->size - index)
                throw std::out_of_range("String out of range");
            strncat(array, text + index, size);
        }

        View get_chars(size_t count, size_t offset = 0) const
        {
            offset += index;
            if (offset >= size)
                return { nullptr, 0 };
            return { text + offset, count + offset > size ? size - offset : count };
        }

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