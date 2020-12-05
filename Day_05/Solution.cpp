#include <cstdio>
#include <Advent.h>

#define MAX_PASS_COUNT 1024

struct Day05_t {
    Int32_t Count;
    Char_t* Passes[MAX_PASS_COUNT];
};

#define ROW_LENGTH 7

Int64_t ComputeRow(Char_t const* Pass) {
    Int64_t Row = 0;
    for (Int32_t i = 0; i < ROW_LENGTH; ++i) {
        if (Pass[i] == 'B') {
            Row += 1i64 << (ROW_LENGTH - i - 1);
        }
    }

    return Row;
}

#define COLUMN_LENGTH 3

Int64_t ComputeColumn(Char_t const* Pass) {
    Int64_t Column = 0;
    for (Int32_t i = 0; i < COLUMN_LENGTH; ++i) {
        if (Pass[7 + i] == 'R') {
            Column += 1i64 << (COLUMN_LENGTH - i - 1);
        }
    }

    return Column;
}

Int64_t SolvePart1(Day05_t* Day) {
    Int64_t MaxSeatId = 0;
    for (Int32_t i = 0; i < Day->Count; ++i) {
        Int64_t Row = ComputeRow(Day->Passes[i]);
        Int64_t Column = ComputeColumn(Day->Passes[i]);

        Int64_t SeatId = (Row * 8) + Column;
        if (SeatId > MaxSeatId) {
            MaxSeatId = SeatId;
        }
    }

    return MaxSeatId;
}

Int64_t SolvePart2(Day05_t* Day) {
    Bool_t SeatIds[MAX_PASS_COUNT] = {};
    for (Int32_t i = 0; i < Day->Count; ++i) {
        Int64_t Row = ComputeRow(Day->Passes[i]);
        Int64_t Column = ComputeColumn(Day->Passes[i]);

        Int64_t SeatId = (Row * 8) + Column;
        SeatIds[SeatId] = true;
    }

    for (Int32_t i = 1; i < MAX_PASS_COUNT - 1; ++i) {
        if (SeatIds[i - 1] && !SeatIds[i] && SeatIds[i + 1]) {
            return i;
        }
    }

    Panic("No solution found.");
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day05_t Day05 = {};
    Char_t* Search = &Input.Data[0];
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            Input.Data[i] = NULL;

            Day05.Count += 1;
            Assert(Day05.Count <= MAX_PASS_COUNT);
            Day05.Passes[Day05.Count - 1] = Search;
            Search = &Input.Data[i + 1];
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day05));
    printf("Part 2: %lld\n", SolvePart2(&Day05));

    return EXIT_SUCCESS;
}
