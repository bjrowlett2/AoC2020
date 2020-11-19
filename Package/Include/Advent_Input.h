#ifndef Advent_Input_h
#define Advent_Input_h 1

#include <cstdio>
#include <cstdlib>

#include "Advent_Types.h"

struct Input_t {
    Char_t* Data;
    Int64_t Length;
};

inline Input_t LoadInput(Char_t const* Path) {
    FILE* File = NULL;
    if (fopen_s(&File, Path, "r") != 0) {
        exit(EXIT_FAILURE);
    }

    if (fseek(File, 0, SEEK_END) != 0) {
        exit(EXIT_FAILURE);
    }

    Int64_t FileSize = ftell(File);
    if (fseek(File, 0, SEEK_SET) != 0) {
        exit(EXIT_FAILURE);
    }

    Char_t* Buffer = new Char_t[FileSize]; // @MemoryLeak
    Int64_t BytesRead = fread(Buffer, sizeof(Char_t), FileSize, File);

    if (ferror(File) != 0) {
        exit(EXIT_FAILURE);
    }

    if (fclose(File) != 0) {
        exit(EXIT_FAILURE);
    }

    Input_t Input = {
        .Data = Buffer,
        .Length = BytesRead
    };

    return Input;
}

#endif // Advent_Input_h
