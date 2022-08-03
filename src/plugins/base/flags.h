#ifndef BASE_FLAGS_H
#define BASE_FLAGS_H
#include <type_traits>

// From www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html
//      wiggling-bits.net/using-enum-classes-as-type-safe-bitmasks

#define ENABLE_FLAGS(x)  \
template<>                           \
struct EnableBitMaskOperators<x>     \
{                                    \
    static const bool enable = true; \
}
  
template<typename Enum>
struct EnableBitMaskOperators
{
    static const bool enable = false;
};

template<typename Enum>
inline typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator |(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<Enum> (
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
    );
}

template<typename Enum>
inline typename std::enable_if<EnableBitMaskOperators<Enum>::enable, bool>::type
operator &(Enum lhs, Enum rhs)
{
    using underlying = typename std::underlying_type<Enum>::type;
    return static_cast<bool> (
        static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs)
    );
}

#endif
