#include <cstdio>
#include <Advent.h>

#define MAX_ADAPTER_COUNT 256

struct Day10_t {
    Int64_t NumAdapters;
    Int64_t Adapters[MAX_ADAPTER_COUNT];
};

void SortAdapters(Day10_t* Day) {
    for (Int64_t i = 0; i < Day->NumAdapters; ++i) {
        for (Int64_t j = i; j < Day->NumAdapters; ++j) {
            if (Day->Adapters[j] < Day->Adapters[i]) {
                Int64_t Temporary = Day->Adapters[i];
                Day->Adapters[i] = Day->Adapters[j];
                Day->Adapters[j] = Temporary;
            }
        }
    }
}

Int64_t SolvePart1(Day10_t* Day) {
    Int64_t OneJoltDifferences = 1;
    Int64_t ThreeJoltDifferences = 1;
    for (Int64_t i = 1; i < Day->NumAdapters; ++i) {
        Int64_t Difference = Day->Adapters[i] - Day->Adapters[i - 1];
        if (Difference == 1) {
            OneJoltDifferences += 1;
        } else if (Difference == 3) {
            ThreeJoltDifferences += 1;
        }
    }

    return OneJoltDifferences * ThreeJoltDifferences;
}

Int64_t SolvePart2(Day10_t* Day) {
    Int64_t Permutations[MAX_ADAPTER_COUNT] = {};
    for (Int64_t i = 0; i < Day->NumAdapters; ++i) {
        if (Day->Adapters[i] <= 3) {
            Permutations[i] = 1;
        }
    }

    for (Int64_t i = 0; i < Day->NumAdapters; ++i) {
        for (Int64_t j = Max(i - 3, 0i64); j < i; ++j) {
            Int64_t Difference = Day->Adapters[i] - Day->Adapters[j];
            if (Difference <= 3) {
                Permutations[i] += Permutations[j];
            }
        }
    }

    return Permutations[Day->NumAdapters - 1];
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day10_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumAdapters += 1;
        Assert(Day.NumAdapters <= MAX_ADAPTER_COUNT);
        Int64_t* Adapter = &Day.Adapters[Day.NumAdapters - 1];

        Offset += ScanForInt(Input.Data + Offset, Adapter);

        Offset += 1; // Eat the newline.
    }

    SortAdapters(&Day);

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
