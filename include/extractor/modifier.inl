/**
 * @file modifier.inl
 * @author your name (you@domain.com)
 * 
 * @brief File with various patterns which can be checked 
 * for in extractor object. These are usualy more complex than
 * the ones in instruction.inl as thay may take another modifier with
 * its own specific pattern as template argument.
 * 
 * 
 * @version 0.1
 * @date 2022-08-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EXTRACTOR_MODIFIER_INL
#define EXTRACTOR_MODIFIER_INL

#include "core.hpp"
#include "extractor.hpp"

namespace extract
{

#pragma region Modifiers

    template <class Modifier,
        size_t Count,
        typename = void>
    struct More;

    template <class Modifier,
        size_t MinCount,
        size_t MaxCount,
        typename = void>
    struct FromTo;

    template <class ModifierEnd,
        bool IncludeEnd = false,
        typename        = void>
    struct Until;

    template <class Modifier1,
        class Modifier2,
        typename = void>
    struct And;

#pragma endregion Modifiers

#pragma region ModifiersImpl

    template <typename Type>
    using is_modifier = std::is_same<
        decltype(Extractor("").test_for<Type>(size_t())),
        size_t>;

    template <class Modifier, size_t Count>
    struct More<
        Modifier,
        Count,
        typename std::enable_if<
            is_modifier<
                Modifier>::value>::type>
    {
        static_assert(Count != 0, "count may not be 0");
        using modifier                = Modifier;
        static constexpr size_t count = Count;

        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            size_t result = 0, end = extractor->remaining() - offset, temp_result;
            for (size_t i = 0; i < count && result < end; i++) {
                temp_result = extractor->test_for<Modifier>(offset + result);
                if (temp_result == 0) {
                    return 0;
                }
                result += temp_result;
            }
            return result;
        }
    };

    template <class Modifier, size_t MinCount, size_t MaxCount>
    struct FromTo<Modifier,
        MinCount,
        MaxCount,
        typename std::enable_if<
            is_modifier<
                Modifier>::value>::type>
    {
        static_assert(MinCount < MaxCount, "maximum count may not be smaller than minimum count");
        using modifier                  = Modifier;
        static constexpr size_t minimum = MinCount;
        static constexpr size_t count   = MaxCount - MinCount;

        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            size_t result = More<Modifier, MinCount>::test_for(extractor, offset);
            if (result == 0) {
                return 0;
            }
            size_t temp_result, end = extractor->remaining() - offset;
            for (size_t i = 0; i < count && result < end; i++) {
                temp_result = extractor->test_for<Modifier>(offset + result);
                if (temp_result == 0) {
                    return result;
                }
                result += temp_result;
            }
            return result;
        }
    };

    template <class ModifierEnd>
    struct Until<ModifierEnd,
        false,
        typename std::enable_if<
            is_modifier<
                ModifierEnd>::value>::type>
    {
        using until_modifier           = ModifierEnd;
        static constexpr bool skip_end = true;

        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            size_t result = 0, end = extractor->remaining(), temp_result;
            for (int i = offset; i < end; i++) {
                temp_result = extractor->test_for<ModifierEnd>(offset + result);
                if (temp_result != 0) {
                    return result;
                }
                ++result;
            }
        }
    };

    template <class ModifierEnd>
    struct Until<ModifierEnd,
        true,
        typename std::enable_if<
            is_modifier<
                ModifierEnd>::value>::type>
    {
        using until_modifier           = ModifierEnd;
        static constexpr bool skip_end = true;

        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            size_t result = 0, end = extractor->remaining(), temp_result;
            for (int i = offset; i < end; i++) {
                temp_result = extractor->test_for<ModifierEnd>(offset + result);
                if (temp_result != 0) {
                    return result + temp_result;
                }
                ++result;
            }
        }
    };

    template <class Modifier1, class Modifier2>
    struct And<Modifier1,
        Modifier2,
        typename std::enable_if<
            is_modifier<Modifier1>::value
            && is_modifier<Modifier2>::value>::type>
    {
        using first_modifier  = Modifier1;
        using second_modifier = Modifier2;

        inline static size_t test_for(const Extractor* extractor, size_t offset)
        {
            size_t result1 = extractor->test_for<Modifier1>();
            if (result1 == 0)
                return 0;
            size_t result2 = extractor->test_for<Modifier2>();
            if (result2 == 0)
                return 0;
            return result1 + result2;
        }
    };

#pragma endregion ModifiersImpl

#pragma region Unions

    template <class Modfier, size_t count1, size_t count2>
    struct Union<More<Modfier, count1>, More<Modfier, count2>>
    {
        using join = More<Modfier, count1 + count2>;
    };

    template <class Modfier, size_t min1, size_t max1, size_t min2, size_t max2>
    struct Union<FromTo<Modfier, min1, max1>, FromTo<Modfier, min2, max2>>
    {
        using join = FromTo<Modfier, min1 + min2, max1 + max2>;
    };

#pragma endregion Unions

} // namespace extract

#endif
