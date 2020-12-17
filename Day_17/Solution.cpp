#include <cstdio>
#include <Advent.h>

enum State_t {
    ACTIVE,
    INACTIVE
};

struct Coordinate_t {
    Int64_t X;
    Int64_t Y;
    Int64_t Z;
    Int64_t W;
};

Coordinate_t operator+(Coordinate_t A, Coordinate_t B) {
    Coordinate_t Result = {};
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;
    return Result;
}

template <>
Uint32_t Hash(Coordinate_t Key) { // Michael Blurr's hash combine.
    Uint32_t Seed = 4;
    Seed ^= Key.X + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
    Seed ^= Key.Y + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
    Seed ^= Key.Z + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
    Seed ^= Key.W + 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
    return Seed % MAX_HASH_BUCKET_COUNT;
}

template <>
Bool_t Equals(Coordinate_t A, Coordinate_t B) {
    return (A.X == B.X) && (A.Y == B.Y) && (A.Z == B.Z) && (A.W == B.W);
}

#define MAX_GRID_SIZE 16

struct Day17_t {
    Int64_t Width;
    Int64_t Height;
    Char_t Grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
};

typedef HashTable_t<Coordinate_t,  State_t> Universe_t;
typedef HashBucket_t<Coordinate_t, State_t> UniverseBucket_t;
typedef HashEntry_t<Coordinate_t,  State_t> UniverseBucketEntry_t;

typedef HashTable_t<Coordinate_t,  Int64_t> Neighborhood_t;
typedef HashBucket_t<Coordinate_t, Int64_t> NeighborhoodBucket_t;
typedef HashEntry_t<Coordinate_t,  Int64_t> NeighborhoodBucketEntry_t;

Universe_t* Simulate(Universe_t* Cubes, Int64_t NumOffsets, Coordinate_t const* Offsets) {
    Neighborhood_t* Sums = new Neighborhood_t();
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        UniverseBucket_t* Bucket = &Cubes->Buckets[i];

        for (Int64_t j = 0; j < Bucket->NumEntries; ++j) {
            UniverseBucketEntry_t* Entry = &Bucket->Entries[j];

            for (Int64_t k = 0; k < NumOffsets; ++k) {
                Int64_t NumNeighbors = 1;
                Coordinate_t Neighbor = Entry->Key + Offsets[k];
                if (Find(Sums, Neighbor, &NumNeighbors)) {
                    NumNeighbors += 1;
                }

                Add(Sums, Neighbor, NumNeighbors);
            }
        }
    }

    Universe_t* Next = new Universe_t();
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        NeighborhoodBucket_t* Bucket = &Sums->Buckets[i];

        for (Int64_t j = 0; j < Bucket->NumEntries; ++j) {
            NeighborhoodBucketEntry_t* Entry = &Bucket->Entries[j];

            State_t State = {};
            if (Find(Cubes, Entry->Key, &State)) {
                if ((Entry->Value == 2) || (Entry->Value == 3)) {
                    Add(Next, Entry->Key, ACTIVE);
                }
            } else {
                if (Entry->Value == 3) {
                    Add(Next, Entry->Key, ACTIVE);
                }
            }
        }
    }

    delete Sums;
    delete Cubes;
    return Next;
}


Int64_t SolvePart1(Day17_t* Day) {
    Universe_t* Cubes = new Universe_t();
    for (Int64_t j = 0; j < Day->Height; ++j) {
        for (Int64_t i = 0; i < Day->Width; ++i) {
            if (Day->Grid[j][i] == '#') {
                Coordinate_t Coordinate = { i, j };
                Add(Cubes, Coordinate, ACTIVE);
            }
        }
    }

    Int64_t const NumOffsets = 26;
    Coordinate_t Offsets[NumOffsets] = {
        { -1, -1, -1,  0 },
        { -1, -1,  0,  0 },
        { -1, -1, +1,  0 },
        { -1,  0, -1,  0 },
        { -1,  0,  0,  0 },
        { -1,  0, +1,  0 },
        { -1, +1, -1,  0 },
        { -1, +1,  0,  0 },
        { -1, +1, +1,  0 },
        {  0, -1, -1,  0 },
        {  0, -1,  0,  0 },
        {  0, -1, +1,  0 },
        {  0,  0, -1,  0 },
        {  0,  0, +1,  0 },
        {  0, +1, -1,  0 },
        {  0, +1,  0,  0 },
        {  0, +1, +1,  0 },
        { +1, -1, -1,  0 },
        { +1, -1,  0,  0 },
        { +1, -1, +1,  0 },
        { +1,  0, -1,  0 },
        { +1,  0,  0,  0 },
        { +1,  0, +1,  0 },
        { +1, +1, -1,  0 },
        { +1, +1,  0,  0 },
        { +1, +1, +1,  0 },
    };

    for (Int64_t Cycle = 0; Cycle < 6; ++Cycle) {
        Cubes = Simulate(Cubes, NumOffsets, Offsets);
    }

    Int64_t Total = 0;
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        Total += Cubes->Buckets[i].NumEntries;
    }

    return Total;
}

Int64_t SolvePart2(Day17_t* Day) {
    Universe_t* Cubes = new Universe_t();
    for (Int64_t j = 0; j < Day->Height; ++j) {
        for (Int64_t i = 0; i < Day->Width; ++i) {
            if (Day->Grid[j][i] == '#') {
                Coordinate_t Coordinate = { i, j };
                Add(Cubes, Coordinate, ACTIVE);
            }
        }
    }

    Int64_t const NumOffsets = 80;
    Coordinate_t Offsets[NumOffsets] = {
        { -1, -1, -1, -1 },
        { -1, -1, -1,  0 },
        { -1, -1, -1, +1 },
        { -1, -1,  0, -1 },
        { -1, -1,  0,  0 },
        { -1, -1,  0, +1 },
        { -1, -1, +1, -1 },
        { -1, -1, +1,  0 },
        { -1, -1, +1, +1 },
        { -1,  0, -1, -1 },
        { -1,  0, -1,  0 },
        { -1,  0, -1, +1 },
        { -1,  0,  0, -1 },
        { -1,  0,  0,  0 },
        { -1,  0,  0, +1 },
        { -1,  0, +1, -1 },
        { -1,  0, +1,  0 },
        { -1,  0, +1, +1 },
        { -1, +1, -1, -1 },
        { -1, +1, -1,  0 },
        { -1, +1, -1, +1 },
        { -1, +1,  0, -1 },
        { -1, +1,  0,  0 },
        { -1, +1,  0, +1 },
        { -1, +1, +1, -1 },
        { -1, +1, +1,  0 },
        { -1, +1, +1, +1 },
        {  0, -1, -1, -1 },
        {  0, -1, -1,  0 },
        {  0, -1, -1, +1 },
        {  0, -1,  0, -1 },
        {  0, -1,  0,  0 },
        {  0, -1,  0, +1 },
        {  0, -1, +1, -1 },
        {  0, -1, +1,  0 },
        {  0, -1, +1, +1 },
        {  0,  0, -1, -1 },
        {  0,  0, -1,  0 },
        {  0,  0, -1, +1 },
        {  0,  0,  0, -1 },
        {  0,  0,  0, +1 },
        {  0,  0, +1, -1 },
        {  0,  0, +1,  0 },
        {  0,  0, +1, +1 },
        {  0, +1, -1, -1 },
        {  0, +1, -1,  0 },
        {  0, +1, -1, +1 },
        {  0, +1,  0, -1 },
        {  0, +1,  0,  0 },
        {  0, +1,  0, +1 },
        {  0, +1, +1, -1 },
        {  0, +1, +1,  0 },
        {  0, +1, +1, +1 },
        { +1, -1, -1, -1 },
        { +1, -1, -1,  0 },
        { +1, -1, -1, +1 },
        { +1, -1,  0, -1 },
        { +1, -1,  0,  0 },
        { +1, -1,  0, +1 },
        { +1, -1, +1, -1 },
        { +1, -1, +1,  0 },
        { +1, -1, +1, +1 },
        { +1,  0, -1, -1 },
        { +1,  0, -1,  0 },
        { +1,  0, -1, +1 },
        { +1,  0,  0, -1 },
        { +1,  0,  0,  0 },
        { +1,  0,  0, +1 },
        { +1,  0, +1, -1 },
        { +1,  0, +1,  0 },
        { +1,  0, +1, +1 },
        { +1, +1, -1, -1 },
        { +1, +1, -1,  0 },
        { +1, +1, -1, +1 },
        { +1, +1,  0, -1 },
        { +1, +1,  0,  0 },
        { +1, +1,  0, +1 },
        { +1, +1, +1, -1 },
        { +1, +1, +1,  0 },
        { +1, +1, +1, +1 },
    };

    for (Int64_t Cycle = 0; Cycle < 6; ++Cycle) {
        Cubes = Simulate(Cubes, NumOffsets, Offsets);
    }

    Int64_t Total = 0;
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        Total += Cubes->Buckets[i].NumEntries;
    }

    return Total;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day17_t Day = {};
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
