#include <cstdio>
#include <Advent.h>

#define MAX_ENTRY_COUNT 256

struct Day01_t {
    Int64_t NumEntries;
    Int64_t Entries[MAX_ENTRY_COUNT];
};

Int64_t SolvePart1(Day01_t* Day) {
    for (Int64_t i = 0; i < Day->NumEntries; ++i) {
        for (Int64_t j = i; j < Day->NumEntries; ++j) {
            if (Day->Entries[i] + Day->Entries[j] == 2020) {
                return Day->Entries[i] * Day->Entries[j];
            }
        }
    }

    Panic("No solution found");
}

Int64_t SolvePart2(Day01_t* Day) {
    for (Int64_t i = 0; i < Day->NumEntries; ++i) {
        for (Int64_t j = i; j < Day->NumEntries; ++j) {
            for (Int64_t k = j; k < Day->NumEntries; ++k) {
                if (Day->Entries[i] + Day->Entries[j] + Day->Entries[k] == 2020) {
                    return Day->Entries[i] * Day->Entries[j] * Day->Entries[k];
                }
            }
        }
    }

    Panic("No solution found");
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day01_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumEntries += 1;
        Assert(Day.NumEntries <= MAX_ENTRY_COUNT);
        Int64_t* Entry = &Day.Entries[Day.NumEntries - 1];

        Offset += ScanForInt(Input.Data + Offset, Entry);

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));
    
    return EXIT_SUCCESS;
}
