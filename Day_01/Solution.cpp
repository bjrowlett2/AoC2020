#include <cstdio>
#include <Advent.h>

#define MAX_ENTRY_COUNT 256

struct Day01_t {
    Int32_t Count;
    Int32_t Entries[MAX_ENTRY_COUNT];
};

Int64_t SolvePart1(Day01_t* Day) {
    for (Int32_t i = 0; i < Day->Count; ++i) {
        for (Int32_t j = i; j < Day->Count; ++j) {
            if (Day->Entries[i] + Day->Entries[j] == 2020) {
                return Day->Entries[i] * Day->Entries[j];
            }
        }
    }

    Panic("No solution found");
}

Int64_t SolvePart2(Day01_t* Day) {
    for (Int32_t i = 0; i < Day->Count; ++i) {
        for (Int32_t j = i; j < Day->Count; ++j) {
            for (Int32_t k = j; k < Day->Count; ++k) {
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

    Day01_t Day01 = {};
    Char_t* String = Input.Data;
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            Input.Data[i] = NULL;

            Day01.Count += 1;
            Assert(Day01.Count <= MAX_ENTRY_COUNT);
            Day01.Entries[Day01.Count - 1] = atoi(String);

            String = &Input.Data[i + 1];
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day01));
    printf("Part 2: %lld\n", SolvePart2(&Day01));
    
    return EXIT_SUCCESS;
}
