#ifndef Advent_Input_h
#define Advent_Input_h 1

#include <cstdio>
#include <cstdlib>

#include "Advent_Types.h"

struct Input_t {
    Char_t* Data;
    Int64_t Length;
};

inline Bool_t LoadInput(Input_t* Input, Char_t const* Path) {
    Assert(Input != NULL);

    FILE* File = NULL;
    if (fopen_s(&File, Path, "r") != 0) {
        return false;
    }

    if (fseek(File, 0, SEEK_END) != 0) {
        return false;
    }

    Int64_t FileSize = ftell(File);
    if (fseek(File, 0, SEEK_SET) != 0) {
        return false;
    }

    Char_t* Buffer = new Char_t[FileSize]; // @MemoryLeak
    Int64_t BytesRead = fread(Buffer, sizeof(Char_t), FileSize, File);

    if (ferror(File) != 0) {
        return false;
    }

    if (fclose(File) != 0) {
        return false;
    }

    Input->Data = Buffer;
    Input->Length = BytesRead;
    return true;
}

#endif // Advent_Input_h
