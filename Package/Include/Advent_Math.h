#ifndef Advent_Math_h
#define Advent_Math_h 1

template <typename Data_t>
Data_t Abs(Data_t Value) {
    return (Value >= 0) ? Value : -Value;
}

template <typename Data_t>
Data_t Max(Data_t A, Data_t B) {
    return (A >= B) ? A : B;
}

#endif // Advent_Math_h
