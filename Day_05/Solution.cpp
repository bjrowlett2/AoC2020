#include <cstdio>
#include <Advent.h>

#define ROW_LENGTH 7
#define COLUMN_LENGTH 3

struct Pass_t {
    Char_t Row[ROW_LENGTH];
    Char_t Column[COLUMN_LENGTH];
};

#define MAX_PASS_COUNT 1024

struct Day05_t {
    Int64_t NumPasses;
    Pass_t Passes[MAX_PASS_COUNT];
};

Int64_t ComputeRow(Pass_t* Pass) {
    Int64_t Row = 0;
    for (Int64_t i = 0; i < ROW_LENGTH; ++i) {
        if (Pass->Row[i] == 'B') {
            Row += 1i64 << (ROW_LENGTH - i - 1);
        }
    }

    return Row;
}

Int64_t ComputeColumn(Pass_t* Pass) {
    Int64_t Column = 0;
    for (Int64_t i = 0; i < COLUMN_LENGTH; ++i) {
        if (Pass->Column[i] == 'R') {
            Column += 1i64 << (COLUMN_LENGTH - i - 1);
        }
    }

    return Column;
}

Int64_t SolvePart1(Day05_t* Day) {
    Int64_t MaxSeatId = 0;
    for (Int64_t i = 0; i < Day->NumPasses; ++i) {
        Int64_t Row = ComputeRow(&Day->Passes[i]);
        Int64_t Column = ComputeColumn(&Day->Passes[i]);

        Int64_t SeatId = (Row * 8) + Column;
        if (SeatId > MaxSeatId) {
            MaxSeatId = SeatId;
        }
    }

    return MaxSeatId;
}

Int64_t SolvePart2(Day05_t* Day) {
    Bool_t SeatIds[MAX_PASS_COUNT] = {};
    for (Int64_t i = 0; i < Day->NumPasses; ++i) {
        Int64_t Row = ComputeRow(&Day->Passes[i]);
        Int64_t Column = ComputeColumn(&Day->Passes[i]);

        Int64_t SeatId = (Row * 8) + Column;
        SeatIds[SeatId] = true;
    }

    for (Int64_t i = 1; i < MAX_PASS_COUNT - 1; ++i) {
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

    Day05_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumPasses += 1;
        Assert(Day.NumPasses <= MAX_PASS_COUNT);
        Pass_t* Pass = &Day.Passes[Day.NumPasses - 1];

        for (Int64_t i = 0; i < ROW_LENGTH; ++i, ++Offset) {
            Pass->Row[i] = Input.Data[Offset];
        }

        for (Int64_t i = 0; i < COLUMN_LENGTH; ++i, ++Offset) {
            Pass->Column[i] = Input.Data[Offset];
        }

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
