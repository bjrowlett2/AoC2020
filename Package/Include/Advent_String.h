#ifndef Advent_String_h
#define Advent_String_h 1

#include "Advent_Panic.h"
#include "Advent_Types.h"

Bool_t IsAlpha(Char_t C) {
    return (('a' <= C) && (C <= 'z'))
        || (('A' <= C) && (C <= 'Z'));
}

Bool_t IsDigit(Char_t C) {
    return ('0' <= C) && (C <= '9');
}

Bool_t IsHexDigit(Char_t C) {
    return (('0' <= C) && (C <= '9'))
        || (('a' <= C) && (C <= 'f'));
}

Bool_t IsAlphaNumeric(Char_t C) {
    return IsAlpha(C) || IsDigit(C);
}

Bool_t IsWhitespace(Char_t C) {
    return (C == ' ')
        || (C == '\n')
        || (C == '\r')
        || (C == '\t');
}

Int64_t StringLength(Char_t const* A) {
    Int64_t Length = 0;
    while (A[Length] != NULL) {
        Length += 1;
    }

    return Length;
}

Bool_t StringCopy(Char_t* A, Char_t const* B) {
    Int64_t LengthA = StringLength(A);
    Int64_t LengthB = StringLength(B);

    if (LengthA <= LengthB) {
        for (Int64_t Index = 0; Index < LengthB; ++Index) {
            A[Index] = B[Index];
        }

        return true;
    }

    return false;
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

Int64_t ScanForInt(Char_t* Buffer, Int64_t* Value) {
    Char_t* End = NULL;
    *Value = strtol(Buffer, &End, 10);

    Assert(Buffer != End);
    return End - Buffer;
}

Int64_t ScanForString(Char_t* Buffer, Int64_t BufferLength, Char_t* Value, Int64_t* Length) {
    for (Int64_t Index = 0; Index < BufferLength-1; ++Index) {
        if (IsWhitespace(Buffer[Index])) {
            *Length = Index;
            return Index;
        }

        Value[Index] = Buffer[Index];
    }

    Panic("Could not find a string.");
}

Int64_t ScanForEnglish(Char_t* Buffer, Int64_t BufferLength, Char_t* Value, Int64_t* Length) {
    for (Int64_t Index = 0; Index < BufferLength-1; ++Index) {
        if (!IsAlpha(Buffer[Index])) {
            *Length = Index;
            return Index;
        }

        Value[Index] = Buffer[Index];
    }

    Panic("Could not find an English word.");
}

#endif // Advent_String_h
