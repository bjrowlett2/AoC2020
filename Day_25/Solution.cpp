#include <cstdio>
#include <Advent.h>

struct Day24_t {
    Int64_t CardPublicKey;
    Int64_t DoorPublicKey;
};

Int64_t FindLoopSize(Int64_t PublicKey) {
    Int64_t Value = 1;
    Int64_t LoopSize = 0;
    while (Value != PublicKey) {
        Value = (Value * 7) % 20201227;
        LoopSize += 1;
    }

    return LoopSize;
}

Int64_t TransformSecretNumber(Int64_t LoopSize, Int64_t SubjectNumber) {
    Int64_t Value = 1;
    for (Int64_t i = 0; i < LoopSize; ++i) {
        Value = (Value * SubjectNumber) % 20201227;
    }

    return Value;
}

Int64_t SolvePart1(Day24_t* Day) {
    Int64_t CardLoopSize = FindLoopSize(Day->CardPublicKey);
    return TransformSecretNumber(CardLoopSize, Day->DoorPublicKey);
}

Int64_t SolvePart2(Day24_t* Day) {
    return 0;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day24_t Day = {};
    Int64_t Offset = 0;
    while (Input.Data[Offset] != '\n') {
        Offset += ScanForInt(Input.Data + Offset, &Day.CardPublicKey);
        Offset += ScanForInt(Input.Data + Offset, &Day.DoorPublicKey);
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;

}