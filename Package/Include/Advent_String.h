#ifndef Advent_String_h
#define Advent_String_h 1

#include "Advent_Types.h"

Bool_t IsDigit(Char_t C) {
    return ('0' <= C) && (C <= '9');
}

Bool_t IsHexDigit(Char_t C) {
    return (('0' <= C) && (C <= '9'))
        || (('a' <= C) && (C <= 'f'));
}

Bool_t IsLetter(Char_t C) {
    return ('a' <= C) && (C <= 'z');
}

Int64_t StringLength(Char_t const* A) {
    Int64_t Length = 0;
    while (A[Length] != NULL) {
        Length += 1;
    }

    return Length;
}

Bool_t StringEquals(Char_t const* A, Char_t const* B) {
    if (A && B) {
        while (*A && *B && (*A == *B)) {
            A += 1;
            B += 1;
        }

        return (*A == NULL) && (*B == NULL);
    }

    return false;
}

Bool_t StringStartsWith(Char_t const* A, Char_t const* B) {
    if (A && B) {
        while (*A && *B && (*A == *B)) {
            A += 1;
            B += 1;
        }

        return (*B == NULL);
    }

    return false;
}

#endif // Advent_String_h
