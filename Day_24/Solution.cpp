#include <cstdio>
#include <Advent.h>

enum Color_t {
    White,
    Black,
};

enum Direction_t {
    East,
    SouthEast,
    SouthWest,
    West,
    NorthWest,
    NorthEast,
};

#define MAX_DIRECTION_COUNT 24

struct Line_t {
    Int64_t NumDirections;
    Direction_t Directions[MAX_DIRECTION_COUNT];
};

#define MAX_LINE_COUNT 512

struct Day24_t {
    Int64_t NumLines;
    Line_t Lines[MAX_LINE_COUNT];
};

struct Coordinate_t {
    Int64_t X;
    Int64_t Y;
};

Coordinate_t operator+(Coordinate_t A, Coordinate_t B) {
    Coordinate_t Result = {};
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return Result;
}

template <>
Uint32_t Hash(Coordinate_t Key) { // Michael Blurr's hash combine.
    Uint32_t Seed = 2;
    Seed ^= Key.X + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
    Seed ^= Key.Y + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
    return Seed % MAX_HASH_BUCKET_COUNT;
}

template <>
Bool_t Equals(Coordinate_t A, Coordinate_t B) {
    return (A.X == B.X) && (A.Y == B.Y);
}

typedef HashTable_t<Coordinate_t,  Color_t> Floor_t;
typedef HashBucket_t<Coordinate_t, Color_t> FloorBucket_t;
typedef HashEntry_t<Coordinate_t,  Color_t> FloorBucketEntry_t;

void PlaceTiles(Day24_t* Day, Floor_t* Floor) {
    for (Int64_t i = 0; i < Day->NumLines; ++i) {
        Line_t* Line = &Day->Lines[i];

        Coordinate_t Coordinate = {};
        for (Int64_t j = 0; j < Line->NumDirections; ++j) {
            Direction_t* Direction = &Line->Directions[j];

            if (*Direction == East) {
                Coordinate.X += 2;
            } else if (*Direction == SouthEast) {
                Coordinate.X += 1;
                Coordinate.Y -= 1;
            } else if (*Direction == SouthWest) {
                Coordinate.X -= 1;
                Coordinate.Y -= 1;
            } else if (*Direction == West) {
                Coordinate.X -= 2;
            } else if (*Direction == NorthWest) {
                Coordinate.X -= 1;
                Coordinate.Y += 1;
            } else if (*Direction == NorthEast) {
                Coordinate.X += 1;
                Coordinate.Y += 1;
            }
        }

        Color_t Color = {};
        if (Find(Floor, Coordinate, &Color)) {
            if (Color == White) {
                Add(Floor, Coordinate, Black);
            } else if (Color == Black) {
                Add(Floor, Coordinate, White);
            }
        } else {
            Add(Floor, Coordinate, Black);
        }
    }
}

Int64_t CountTiles(Floor_t* Floor) {
    Int64_t Count = 0;
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        FloorBucket_t* Bucket = &Floor->Buckets[i];

        for (Int64_t j = 0; j < Bucket->NumEntries; ++j) {
            FloorBucketEntry_t* Entry = &Bucket->Entries[j];

            if (Entry->Value == Black) {
                Count += 1;
            }
        }
    }

    return Count;
}

Int64_t SolvePart1(Day24_t* Day) {
    Floor_t* Floor = new Floor_t();

    PlaceTiles(Day, Floor);
    return CountTiles(Floor);
}

Int64_t CountNeighbor(Floor_t* Floor, Coordinate_t Coordinate) {
    Color_t Color = {};
    if (Find(Floor, Coordinate, &Color)) {
        if (Color == Black) {
            return 1;
        }
    }

    return 0;
}

Int64_t CountNeighbors(Floor_t* Floor, Coordinate_t Coordinate) {
    return CountNeighbor(Floor, Coordinate + Coordinate_t {  2,  0 })
         + CountNeighbor(Floor, Coordinate + Coordinate_t {  1, -1 })
         + CountNeighbor(Floor, Coordinate + Coordinate_t { -1, -1 })
         + CountNeighbor(Floor, Coordinate + Coordinate_t { -2,  0 })
         + CountNeighbor(Floor, Coordinate + Coordinate_t { -1,  1 })
         + CountNeighbor(Floor, Coordinate + Coordinate_t {  1,  1 });
}

Bool_t ShouldFlip(Floor_t* Floor, Coordinate_t Coordinate, Color_t Color) {
    Int64_t Neighbors = CountNeighbors(Floor, Coordinate);

    if (Color == Black) {
        return ((Neighbors == 0) || (Neighbors > 2));
    } else if (Color == White) {
        return (Neighbors == 2);
    }

    return false;
}

void ProcessTile(Floor_t* Previous, Floor_t* Current, Coordinate_t Coordinate, Color_t Color) {
    if (ShouldFlip(Previous, Coordinate, Color)) {
        if (Color == White) {
            Add(Current, Coordinate, Black);
        } else if (Color == Black) {
            Add(Current, Coordinate, White);
        }
    }
}

void ProcessNeighbor(Floor_t* Previous, Floor_t* Current, Coordinate_t Origin, Int64_t Dx, Int64_t Dy) {
    Coordinate_t Neighbor=
        Origin + Coordinate_t { Dx, Dy };

    Color_t Color = White;
    Find(Previous, Neighbor, &Color);
    ProcessTile(Previous, Current, Neighbor, Color);
}

void RearrangeTiles(Floor_t* Floor) {
    Floor_t* Previous = new Floor_t();
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        FloorBucket_t* Bucket = &Floor->Buckets[i];

        for (Int64_t j = 0; j < Bucket->NumEntries; ++j) {
            FloorBucketEntry_t* Entry = &Bucket->Entries[j];
            Add(Previous, Entry->Key, Entry->Value);
        }
    }

    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        FloorBucket_t* Bucket = &Previous->Buckets[i];

        for (Int64_t j = 0; j < Bucket->NumEntries; ++j) {
            FloorBucketEntry_t* Entry = &Bucket->Entries[j];

            { // This tile.
                Color_t Color = White;
                Find(Previous, Entry->Key, &Color);
                ProcessTile(Previous, Floor, Entry->Key, Color);
            }

            // @Speed: This is inefficient!
            // We are updating the same tile multiple times...

            ProcessNeighbor(Previous, Floor, Entry->Key,  2,  0);
            ProcessNeighbor(Previous, Floor, Entry->Key,  1, -1);
            ProcessNeighbor(Previous, Floor, Entry->Key, -1, -1);
            ProcessNeighbor(Previous, Floor, Entry->Key, -2,  0);
            ProcessNeighbor(Previous, Floor, Entry->Key, -1,  1);
            ProcessNeighbor(Previous, Floor, Entry->Key,  1,  1);
        }
    }

    delete Previous;
}

Int64_t SolvePart2(Day24_t* Day) {
    Floor_t* Floor = new Floor_t();

    PlaceTiles(Day, Floor);
    for (Int64_t i = 0; i < 100; ++i) {
        RearrangeTiles(Floor);
    }

    return CountTiles(Floor);
}

Int64_t LoadLine(Input_t* Input, Int64_t Base, Line_t* Line) {
    Int64_t Offset = 0;
    while (Input->Data[Base + Offset] != '\n') {
        Line->NumDirections += 1;
        Assert(Line->NumDirections <= MAX_DIRECTION_COUNT);
        Direction_t* Direction = &Line->Directions[Line->NumDirections - 1];
        
        if (Input->Data[Base + Offset] == 'e') {
            *Direction = East;

            Offset += 1; // Eat the direction.
        } else if (Input->Data[Base + Offset] == 's') {
            if (Input->Data[Base + Offset + 1] == 'e') {
                *Direction = SouthEast;

                Offset += 2; // Eat the direction.
            } else if (Input->Data[Base + Offset + 1] == 'w') {
                *Direction = SouthWest;

                Offset += 2; // Eat the direction.
            }
        } else if (Input->Data[Base + Offset] == 'w') {
            *Direction = West;

            Offset += 1; // Eat the direction.
        } else if (Input->Data[Base + Offset] == 'n') {
            if (Input->Data[Base + Offset + 1] == 'e') {
                *Direction = NorthEast;

                Offset += 2; // Eat the direction.
            } else if (Input->Data[Base + Offset + 1] == 'w') {
                *Direction = NorthWest;

                Offset += 2; // Eat the direction.
            }
        }
    }

    Offset += 1; // Eat the newline.

    return Offset;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day24_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumLines += 1;
        Assert(Day.NumLines <= MAX_LINE_COUNT);
        Line_t* Line = &Day.Lines[Day.NumLines - 1];

        Offset += LoadLine(&Input, Offset, Line);
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;

}