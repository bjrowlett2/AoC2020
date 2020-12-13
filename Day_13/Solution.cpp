#include <cstdio>
#include <Advent.h>

struct Bus_t {
    Int64_t BusId;
    Int64_t Position;
};

#define MAX_BUS_COUNT 128

struct Day13_t {
    Int64_t Timestamp;

    Int64_t NumBusses;
    Bus_t Busses[MAX_BUS_COUNT];
};

Int64_t SolvePart1(Day13_t* Day) {
    Int64_t BusId = -1;
    Int64_t MinutesToWait = INT64_MAX;
    for (Int64_t i = 0; i < Day->NumBusses; ++i) {
        Bus_t* Bus = &Day->Busses[i];

        if (Bus->BusId != -1) {
            Int64_t Remainder = Day->Timestamp % Bus->BusId;
            Int64_t MinutesPastTimestamp = Bus->BusId - Remainder;

            if (MinutesPastTimestamp < MinutesToWait) {
                BusId = Bus->BusId;
                MinutesToWait = MinutesPastTimestamp;
            }
        }
    }

    Assert(BusId != -1);
    return BusId * MinutesToWait;
}

Int64_t SolvePart2(Day13_t* Day) {
    Int64_t Product = 1;
    for (Int64_t i = 0; i < Day->NumBusses; ++i) {
        Bus_t* Bus = &Day->Busses[i];

        if (Bus->BusId != -1) {
            Product *= Bus->BusId;
        }
    }

    Int64_t X = 0;
    for (Int64_t i = 0; i < Day->NumBusses; ++i) {
        Bus_t* Bus = &Day->Busses[i];

        if (Bus->BusId != -1) {
            Int64_t P = Product / Bus->BusId;
            Gcd_t Gcd = ExtendedGcd(Bus->BusId, P);

            X += (Bus->BusId - Bus->Position) * Gcd.Y * P;
        }
    }

    Int64_t Timestamp = X % Product;
    if (Timestamp < 0) {
        Timestamp += Product;
    }

    return Timestamp;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day13_t Day = {};
    Int64_t NextPosition = 0;
    Int64_t Offset = ScanForInt(Input.Data, &Day.Timestamp);

    Offset += 1; // Eat the newline.

    while (Offset < Input.Length) {
        Day.NumBusses += 1;
        Assert(Day.NumBusses <= MAX_BUS_COUNT);
        Bus_t* Bus = &Day.Busses[Day.NumBusses - 1];

        Bus->Position = NextPosition++;

        if (Input.Data[Offset] == 'x') {
            Bus->BusId = -1;

            Offset += 2; // Eat the x and the comma.
        } else {
            Offset += ScanForInt(Input.Data + Offset, &Bus->BusId);

            Offset += 1; // Eat the comma.
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
