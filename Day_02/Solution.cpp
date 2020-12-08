#include <cstdio>
#include <Advent.h>

#define MAX_PASSWORD_LENGTH 32

struct Entry_t {
    Int64_t Min;
    Int64_t Max;
    Char_t Letter;

    Int64_t PasswordLength;
    Char_t Password[MAX_PASSWORD_LENGTH];
};

#define MAX_ENTRY_COUNT 1024

struct Day02_t {
    Int64_t NumEntries;
    Entry_t Entries[MAX_ENTRY_COUNT];
};

Int64_t SolvePart1(Day02_t* Day) {
    Int64_t ValidPasswords = 0;
    for (Int64_t i = 0; i < Day->NumEntries; ++i) {
        Entry_t* Entry = &Day->Entries[i];

        Int64_t Occurrences = 0;
        for (Int64_t j = 0; j < Entry->PasswordLength; ++j) {
            if (Entry->Password[j] == Entry->Letter) {
                Occurrences += 1;
            }
        }

        if ((Entry->Min <= Occurrences)
            && (Occurrences <= Entry->Max)) {
                ValidPasswords += 1;
        }
    }

    return ValidPasswords;
}

Int64_t SolvePart2(Day02_t* Day) {
    Int64_t ValidPasswords = 0;
    for (Int64_t i = 0; i < Day->NumEntries; ++i) {
        Entry_t* Entry = &Day->Entries[i];

        Int64_t Min = Entry->Min - 1;
        Int64_t Max = Entry->Max - 1;
        if ((Entry->Password[Min] == Entry->Letter)
            ^ (Entry->Password[Max] == Entry->Letter)) {
                ValidPasswords += 1;
        }
    }

    return ValidPasswords;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day02_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumEntries += 1;
        Assert(Day.NumEntries <= MAX_ENTRY_COUNT);
        Entry_t* Entry = &Day.Entries[Day.NumEntries - 1];

        Offset += ScanForInt(Input.Data + Offset, &Entry->Min);

        Offset += 1; // Eat the hyphen.
        Offset += ScanForInt(Input.Data + Offset, &Entry->Max);

        Offset += 1; // Eat the space.
        Entry->Letter = Input.Data[Offset++];

        Offset += 2; // Eat the colon and the space.
        Offset += ScanForString(Input.Data + Offset, MAX_PASSWORD_LENGTH, Entry->Password, &Entry->PasswordLength);

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));
    
    return EXIT_SUCCESS;
}
