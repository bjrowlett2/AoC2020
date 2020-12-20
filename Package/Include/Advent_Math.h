#ifndef Advent_Math_h
#define Advent_Math_h 1

#include "Advent_Types.h"

template <typename Data_t>
Data_t Abs(Data_t Value) {
    return (Value >= 0) ? Value : -Value;
}

template <typename Data_t>
Data_t Max(Data_t A, Data_t B) {
    return (A >= B) ? A : B;
}

struct Gcd_t {
    Int64_t R;
    Int64_t X;
    Int64_t Y;
};

Int64_t Truncate(Float64_t Value) {
    return static_cast<Int64_t>(Value);
}

Gcd_t ExtendedGcd(Int64_t A, Int64_t B) {
    if (A == 0) {
        return { B, 0, 1 };
    }

    Gcd_t Gcd = ExtendedGcd(B % A, A);
    return { Gcd.R, Gcd.Y - (B / A) * Gcd.X, Gcd.X };
}

#endif // Advent_Math_h
