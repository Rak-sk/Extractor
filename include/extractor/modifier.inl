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

#pragma endregion Modifiers

#pragma region ModifiersImpl

    template <typename Type>
    using is_modifier = std::is_same<
        decltype(Extractor("").test_for<Type>(int())),
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
            size_t result = 0, end = extractor->remaining(), temp_result;
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
            size_t temp_result, end = extractor->remaining();
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

#pragma endregion ModifiersImpl

} // namespace extract

#endif