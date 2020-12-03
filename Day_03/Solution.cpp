#include <cstdio>
#include <Advent.h>

#define MAX_GRID_WIDTH 64
#define MAX_GRID_HEIGHT 512

struct Day03_t {
    Int32_t Width;
    Int32_t Height;
    Char_t Grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH];
};

Int64_t TraverseMap(Day03_t* Day, Int32_t Dx, Int32_t Dy) {
    Int32_t X = 0;
    Int32_t Y = 0;

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

    Day03_t Day03 = {};
    Int32_t GridX = 0, GridY = 0;
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            GridX = 0;
            GridY += 1;
        } else {
            Day03.Grid[GridY][GridX] = Input.Data[i];

            GridX += 1;
            if (GridX > Day03.Width) {
                Day03.Width = GridX;
            }

            if (GridY > Day03.Height) {
                Day03.Height = GridY;
            }
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day03));
    printf("Part 2: %lld\n", SolvePart2(&Day03));

    return EXIT_SUCCESS;
}

