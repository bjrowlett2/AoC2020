#include <cstdio>
#include <Advent.h>

#define MAX_GRID_SIZE 128

struct Day11_t {
    Int64_t Width;
    Int64_t Height;
    Char_t Grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
};

typedef Int64_t CountOccupied_t(Day11_t* Day, Int64_t X, Int64_t Y);

struct Rules_t {
    Int64_t LowerBound;
    Int64_t UpperBound;
    CountOccupied_t* CountOccupied;
};

Bool_t InBounds(Day11_t* Day, Int64_t X, Int64_t Y) {
    return ((0 <= X) && (X < Day->Width))
        && ((0 <= Y) && (Y < Day->Height));
}

Bool_t IsOccupied(Day11_t* Day, Int64_t X, Int64_t Y) {
    if (!InBounds(Day, X, Y)) {
        return false;
    }

    return Day->Grid[Y][X] == '#';
}

Int64_t CountNeighbors(Day11_t* Day, Int64_t X, Int64_t Y) {
    return IsOccupied(Day, X - 1, Y    )
         + IsOccupied(Day, X,     Y - 1)
         + IsOccupied(Day, X + 1, Y    )
         + IsOccupied(Day, X,     Y + 1)
         + IsOccupied(Day, X - 1, Y - 1)
         + IsOccupied(Day, X + 1, Y - 1)
         + IsOccupied(Day, X + 1, Y + 1)
         + IsOccupied(Day, X - 1, Y + 1);
}

Bool_t Simulate(Day11_t* Day, Rules_t* Rules) {
    Day11_t Copy = *Day; // @Speed
    Bool_t Modified = false;
    for (Int64_t Y = 0; Y < Copy.Height; ++Y) {
        for (Int64_t X = 0; X < Copy.Width; ++X) {
            Int64_t Neighbors = Rules->CountOccupied(&Copy, X, Y);

            if (Copy.Grid[Y][X] == 'L') {
                if (Neighbors == Rules->LowerBound) {
                    Modified = true;
                    Day->Grid[Y][X] = '#';
                }
            } else if (Copy.Grid[Y][X] == '#') {
                if (Neighbors >= Rules->UpperBound) {
                    Modified = true;
                    Day->Grid[Y][X] = 'L';
                }
            }
        }
    }

    return Modified;
}

Int64_t SolvePart1(Day11_t* Day) {
    Rules_t Rules = {};
    Rules.LowerBound = 0;
    Rules.UpperBound = 4;
    Rules.CountOccupied = &CountNeighbors;

    Day11_t Copy = *Day; // @Speed
    while (Simulate(&Copy, &Rules)) {
        // Do nothing...
    }

    Int64_t Occupied = 0;
    for (Int64_t Y = 0; Y < Copy.Height; ++Y) {
        for (Int64_t X = 0; X < Copy.Width; ++X) {
            Occupied += IsOccupied(&Copy, X, Y);
        }
    }

    return Occupied;
}

Bool_t IsVisible(Day11_t* Day, Int64_t X, Int64_t Y, Int64_t Dx, Int64_t Dy) {
    do {
        X += Dx;
        Y += Dy;

        if (Day->Grid[Y][X] == '#') {
            return true;
        } else if (Day->Grid[Y][X] == 'L') {
            return false;
        }
    } while (InBounds(Day, X, Y));
    
    return false;
}

Int64_t CountLineOfSight(Day11_t* Day, Int64_t X, Int64_t Y) {
    return IsVisible(Day, X, Y, -1,  0)
         + IsVisible(Day, X, Y,  0, -1)
         + IsVisible(Day, X, Y, +1,  0)
         + IsVisible(Day, X, Y,  0, +1)
         + IsVisible(Day, X, Y, -1, -1)
         + IsVisible(Day, X, Y, +1, -1)
         + IsVisible(Day, X, Y, +1, +1)
         + IsVisible(Day, X, Y, -1, +1);
}

Int64_t SolvePart2(Day11_t* Day) {
    Rules_t Rules = {};
    Rules.LowerBound = 0;
    Rules.UpperBound = 5;
    Rules.CountOccupied = &CountLineOfSight;

    Day11_t Copy = *Day; // @Speed
    while (Simulate(&Copy, &Rules)) {
        // Do nothing...
    }

    Int64_t Occupied = 0;
    for (Int64_t Y = 0; Y < Copy.Height; ++Y) {
        for (Int64_t X = 0; X < Copy.Width; ++X) {
            Occupied += IsOccupied(&Copy, X, Y);
        }
    }

    return Occupied;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day11_t Day = {};
    Int64_t GridX = 0;
    Int64_t GridY = 0;
    for (Int64_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            GridX = 0;
            
            GridY += 1;
            if (GridY > Day.Height) {
                Day.Height = GridY;
            }
        } else {
            Day.Grid[GridY][GridX] = Input.Data[i];

            GridX += 1;
            if (GridX > Day.Width) {
                Day.Width = GridX;
            }
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
