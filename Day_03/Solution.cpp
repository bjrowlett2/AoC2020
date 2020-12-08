#include <cstdio>
#include <Advent.h>

#define MAX_GRID_WIDTH 64
#define MAX_GRID_HEIGHT 512

struct Day03_t {
    Int64_t Width;
    Int64_t Height;
    Char_t Grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH];
};

Int64_t TraverseMap(Day03_t* Day, Int64_t Dx, Int64_t Dy) {
    Int64_t X = 0;
    Int64_t Y = 0;
    Int64_t Encounters = 0;
    while (Y < Day->Height) {
        if (Day->Grid[Y][X] == '#') {
            Encounters += 1;
        }

        X += Dx;
        X %= Day->Width;
        Y += Dy;
    }

    return Encounters;
}

Int64_t SolvePart1(Day03_t* Day) {
    return TraverseMap(Day, 3, 1);
}

Int64_t SolvePart2(Day03_t* Day) {
    return TraverseMap(Day, 1, 1)
         * TraverseMap(Day, 3, 1)
         * TraverseMap(Day, 5, 1)
         * TraverseMap(Day, 7, 1)
         * TraverseMap(Day, 1, 2);
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day03_t Day = {};
    Int64_t GridX = 0;
    Int64_t GridY = 0;
    for (Int64_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            GridX = 0;
            GridY += 1;
        } else {
            Day.Grid[GridY][GridX] = Input.Data[i];

            GridX += 1;
            if (GridX > Day.Width) {
                Day.Width = GridX;
            }

            if (GridY > Day.Height) {
                Day.Height = GridY;
            }
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}

