#include <cstdio>
#include <Advent.h>

struct Entry_t {
    Int32_t Min;
    Int32_t Max;
    Char_t Letter;

    Int64_t Length;
    Char_t* Password;
};

#define MAX_ENTRY_COUNT 1024

struct Day02_t {
    Int32_t Count;
    Entry_t Entries[MAX_ENTRY_COUNT];
};

Int64_t SolvePart1(Day02_t* Day) {
    Int32_t ValidPasswords = 0;
    for (Int32_t i = 0; i < Day->Count; ++i) {
        Entry_t* Entry = &Day->Entries[i];

        Int32_t Occurrences = 0;
        for (Int32_t j = 0; j < Entry->Length; ++j) {
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
    Int32_t ValidPasswords = 0;
    for (Int32_t i = 0; i < Day->Count; ++i) {
        Entry_t* Entry = &Day->Entries[i];

        Int32_t Min = Entry->Min - 1;
        Int32_t Max = Entry->Max - 1;
        if ((Entry->Password[Min] == Entry->Letter)
            ^ (Entry->Password[Max] == Entry->Letter)) {
                ValidPasswords += 1;
        }
    }

    return ValidPasswords;
}

Bool_t ParseChar(Char_t** String, Char_t* Value) {
    Char_t* Search = *String;
    if (!IsLetter(Search[0])) {
        return false;
    }

    *Value = Search[0];
    *String += 1;
    return true;
}

Bool_t ParseInt(Char_t** String, Int32_t* Value) {
    Char_t* Start = *String;
    Char_t* Search = *String;
    while (IsDigit(Search[0])) {
        Search += 1;
    }

    if (Start == Search) {
        return false;
    }

    /// @Robustness
    /// Since we know the character after the value is always ignored,
    /// we can replace it with a NULL so that atoi can parse the integer.
    ///                                     Brian Rowlett, 2020-12-02
    Search[0] = NULL;

    *Value = atoi(Start);
    *String = Search;
    return true;
}

Bool_t ParseString(Char_t** String, Int64_t* Length, Char_t** Value) {
    Char_t* Start = *String;
    Char_t* Search = *String;
    while (Search[0] != NULL) {
        Search += 1;
    }

    *Length = Search - *String;
    *Value = Start;
    return true;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day02_t Day02 = {};
    Char_t* Start = Input.Data;
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            Input.Data[i] = NULL;

            Day02.Count += 1;
            Assert(Day02.Count <= MAX_ENTRY_COUNT);
            Entry_t* Entry = &Day02.Entries[Day02.Count - 1];

            if (!ParseInt(&Start, &Entry->Min)) {
                Panic("Failed to parse min.");
            }

            Start += 1; // Eat the minus sign.
            if (!ParseInt(&Start, &Entry->Max)) {
                Panic("Failed to parse max.");
            }

            Start += 1; // Eat the space.
            if (!ParseChar(&Start, &Entry->Letter)) {
                Panic("Failed to parse letter.");
            }

            Start += 2; // Eat the colon and the space.
            if (!ParseString(&Start, &Entry->Length, &Entry->Password)) {
                Panic("Failed to parse password.");
            }

            Start = &Input.Data[i + 1];
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day02));
    printf("Part 2: %lld\n", SolvePart2(&Day02));
    
    return EXIT_SUCCESS;
}
