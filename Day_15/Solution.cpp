#include <cstdio>
#include <Advent.h>

#define MAX_NUMBER_COUNT 8

struct Day15_t {
    Int64_t NumNumbers;
    Int64_t Numbers[MAX_NUMBER_COUNT];
};

Int64_t* HugeAllocate(Int64_t Size) {
    return static_cast<Int64_t*>(calloc(Size, sizeof(Int64_t)));
}

Int64_t PlayMemoryGame(Day15_t* Day, Int64_t Limit) {
    Int64_t* Numbers = HugeAllocate(Limit);
    for (Int64_t i = 0; i < Day->NumNumbers - 1; ++i) {
        Int64_t Value = Day->Numbers[i];
        Numbers[Value] = i + 1;
    }

    Int64_t Turn = Day->NumNumbers;
    Int64_t LastSpoken = Day->Numbers[Day->NumNumbers - 1];
    while (Turn < Limit) {
        Int64_t Index = Numbers[LastSpoken];
        Numbers[LastSpoken] = Turn;

        if (Index == 0) {
            LastSpoken = 0;
        } else {
            LastSpoken = Turn - Index;
        }

        Turn += 1;
    }

    delete[] Numbers;
    return LastSpoken;
}

Int64_t SolvePart1(Day15_t* Day) {
    return PlayMemoryGame(Day, 2020);
}

Int64_t SolvePart2(Day15_t* Day) {
    return PlayMemoryGame(Day, 30000000);
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day15_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumNumbers += 1;
        Assert(Day.NumNumbers <= MAX_NUMBER_COUNT);
        Int64_t* Number = &Day.Numbers[Day.NumNumbers - 1];

        Offset += ScanForInt(Input.Data + Offset, Number);
        Offset += 1; // Eat the comma or the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
