#include <cstdio>
#include <Advent.h>

#define MAX_NUMBER_COUNT 1024

struct Day09_t {
    Int64_t NumNumbers;
    Int64_t Numbers[MAX_NUMBER_COUNT];
};

Int64_t SolvePart1(Day09_t* Day) {
    for (Int64_t i = 25; i < Day->NumNumbers; ++i) {
        Int64_t Target = Day->Numbers[i];

        Bool_t Found = false;
        for (Int64_t x = i - 25; x < i; ++x) {
            Int64_t A = Day->Numbers[x];

            for (Int64_t y = x; y < i; ++y) {
                Int64_t B = Day->Numbers[y];

                if ((A + B) == Target) {
                    Found = true;
                    break; // Early exit.
                }
            }

            if (Found) {
                break; // Early exit.
            }
        }
        
        if (!Found) {
            return Target;
        }
    }

    Panic("No solution found.");
}

Int64_t SolvePart2(Day09_t* Day) {
    Int64_t Target = 393911906;
    for (Int64_t i = 0; i < Day->NumNumbers; ++i) {
        Int64_t Sum = Day->Numbers[i];

        Int64_t Minimum = Day->Numbers[i];
        Int64_t Maximum = Day->Numbers[i];
        for (Int64_t j = i + 1; j < Day->NumNumbers; ++j) {
            Sum += Day->Numbers[j];

            if (Day->Numbers[j] < Minimum) {
                Minimum = Day->Numbers[j];
            }

            if (Day->Numbers[j] > Maximum) {
                Maximum = Day->Numbers[j];
            }

            if (Sum == Target) {
                return Minimum + Maximum;
            } else if (Sum > Target) {
                break; // Early exit.
            }
        }
    }

    Panic("No solution found.");
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day09_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumNumbers += 1;
        Assert(Day.NumNumbers <= MAX_NUMBER_COUNT);
        Int64_t* Number = &Day.Numbers[Day.NumNumbers - 1];

        Offset += ScanForInt(Input.Data + Offset, Number);
        
        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
