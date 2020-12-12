#include <cstdio>
#include <Advent.h>

struct Action_t {
    Char_t Direction;
    Int64_t Value;
};

#define MAX_ACTION_COUNT 1024

struct Day12_t {
    Int64_t NumActions;
    Action_t Actions[MAX_ACTION_COUNT];
};

void Clockwise(Char_t* Direction) {
    switch (*Direction) {
        case 'N':  *Direction = 'E';  break;
        case 'E':  *Direction = 'S';  break;
        case 'S':  *Direction = 'W';  break;
        case 'W':  *Direction = 'N';  break;
    }
}

void CounterClockwise(Char_t* Direction) {
    switch (*Direction) {
        case 'N':  *Direction = 'W';  break;
        case 'E':  *Direction = 'N';  break;
        case 'S':  *Direction = 'E';  break;
        case 'W':  *Direction = 'S';  break;
    }
}

void RotateShip(Char_t* Facing, Int64_t Degrees) {
    if (Degrees > 0) {
        while (Degrees > 0) {
            Clockwise(Facing);
            Degrees -= 90;
        }
    } else if (Degrees < 0) {
        while (Degrees < 0) {
            CounterClockwise(Facing);
            Degrees += 90;
        }
    }
}

void MoveShipForward(Int64_t* X, Int64_t* Y, Char_t Direction, Int64_t Amount) {
    switch (Direction) {
        case 'N':  *Y += Amount;  break;
        case 'E':  *X += Amount;  break;
        case 'S':  *Y -= Amount;  break;
        case 'W':  *X -= Amount;  break;
    }
}

Int64_t SolvePart1(Day12_t* Day) {
    Char_t Facing = 'E';
    Int64_t X = 0, Y = 0;
    for (Int64_t i = 0; i < Day->NumActions; ++i) {
        Action_t* Action = &Day->Actions[i];

        switch (Action->Direction) {
            case 'N':  Y += Action->Value;  break;
            case 'E':  X += Action->Value;  break;
            case 'S':  Y -= Action->Value;  break;
            case 'W':  X -= Action->Value;  break;
            case 'L':  RotateShip(&Facing, -Action->Value);  break;
            case 'R':  RotateShip(&Facing, +Action->Value);  break;
            case 'F':  MoveShipForward(&X, &Y, Facing, Action->Value);  break;
        }
    }

    return Abs(X) + Abs(Y);
}

void Clockwise(Int64_t* X, Int64_t* Y) {
    Int64_t Temporary = *X;
    *X = *Y;
    *Y = -Temporary;
}

void CounterClockwise(Int64_t* X, Int64_t* Y) {
    Int64_t Temporary = *Y;
    *Y = *X;
    *X = -Temporary;
}

void RotateWaypoint(Int64_t X, Int64_t Y, Int64_t* Wx, Int64_t* Wy, Int64_t Degrees) {
    // Relative to the origin.
    *Wx -= X;
    *Wy -= Y;

    if (Degrees > 0) {
        while (Degrees > 0) {
            Clockwise(Wx, Wy);
            Degrees -= 90;
        }
    } else if (Degrees < 0) {
        while (Degrees < 0) {
            CounterClockwise(Wx, Wy);
            Degrees += 90;
        }
    }

    // Relative to the ship again.
    *Wx += X;
    *Wy += Y;
}

void MoveShipAndWaypointForward(Int64_t* X, Int64_t* Y, Int64_t* Wx, Int64_t* Wy, Int64_t Amount) {
    Int64_t Dx = (*Wx - *X) * Amount;
    *X += Dx;
    *Wx += Dx;

    Int64_t Dy = (*Wy - *Y) * Amount;
    *Y += Dy;
    *Wy += Dy;
}

Int64_t SolvePart2(Day12_t* Day) {
    Int64_t X = 0, Y = 0;
    Int64_t Wx = 10, Wy = 1;
    for (Int64_t i = 0; i < Day->NumActions; ++i) {
        Action_t* Action = &Day->Actions[i];

        switch (Action->Direction) {
            case 'N':  Wy += Action->Value;  break;
            case 'E':  Wx += Action->Value;  break;
            case 'S':  Wy -= Action->Value;  break;
            case 'W':  Wx -= Action->Value;  break;
            case 'L':  RotateWaypoint(X, Y, &Wx, &Wy, -Action->Value);  break;
            case 'R':  RotateWaypoint(X, Y, &Wx, &Wy, +Action->Value);  break;
            case 'F':  MoveShipAndWaypointForward(&X, &Y, &Wx, &Wy, Action->Value);  break;
        }
    }

    return Abs(X) + Abs(Y);
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day12_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumActions += 1;
        Assert(Day.NumActions <= MAX_ACTION_COUNT);
        Action_t* Action = &Day.Actions[Day.NumActions - 1];

        Action->Direction = Input.Data[Offset++];

        Offset += ScanForInt(Input.Data + Offset, &Action->Value);

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
