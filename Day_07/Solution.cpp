#include <cstdio>
#include <Advent.h>

#define MAX_COLOR_LENGTH 24

struct Content_t {
    Int64_t Quantity;

    Int64_t ColorLength;
    Char_t Color[MAX_COLOR_LENGTH];
};

#define MAX_CONTENT_COUNT 8

struct Rule_t {
    Int64_t ColorLength;
    Char_t Color[MAX_COLOR_LENGTH];

    Int64_t NumContents;
    Content_t Contents[MAX_CONTENT_COUNT];
};

#define MAX_RULE_COUNT 1024

struct Day07_t {
    Int64_t NumRules;
    Rule_t Rules[MAX_RULE_COUNT];
};

Bool_t FindRule(Day07_t* Day, Rule_t** Result, Char_t const* Color) {
    for (Int64_t i = 0; i < Day->NumRules; ++i) {
        Rule_t* Rule = &Day->Rules[i];

        if (StringEquals(Color, Rule->Color)) {
            *Result = Rule;
            return true;
        }
    }

    return false;
}

Bool_t CanContainBag(Day07_t* Day, Rule_t* Rule, Char_t const* Color) {
    for (Int64_t i = 0; i < Rule->NumContents; ++i) {
        Content_t* Content = &Rule->Contents[i];

        if (StringEquals(Color, Content->Color)) {
            return true;
        }

        Rule_t* Next = NULL;
        if (FindRule(Day, &Next, Content->Color)) {
            if (CanContainBag(Day, Next, Color)) {
                return true;
            }
        }
    }

    return false;
}

Int64_t SolvePart1(Day07_t* Day) {
    Int64_t NumBags = 0;
    for (Int64_t i = 0; i < Day->NumRules; ++i) {
        Rule_t* Rule = &Day->Rules[i];

        if (CanContainBag(Day, Rule, "shiny gold")) {
            NumBags += 1;
        }
    }

    return NumBags;
}

Int64_t CountNestedBags(Day07_t* Day, Rule_t* Rule) {
    Int64_t Count = 1;
    for (Int64_t i = 0; i < Rule->NumContents; ++i) {
        Content_t* Content = &Rule->Contents[i];

        Rule_t* Next = NULL;
        if (FindRule(Day, &Next, Content->Color)) {
            Count += Content->Quantity * CountNestedBags(Day, Next);
        }
    }

    return Count;
}

Int64_t SolvePart2(Day07_t* Day) {
    Rule_t* Rule = NULL;
    if (FindRule(Day, &Rule, "shiny gold")) {
        return CountNestedBags(Day, Rule) - 1;
    }

    Panic("No solution found.");
}

Int64_t ScanForColor(Char_t* Buffer, Int64_t BufferLength, Char_t* Value, Int64_t* Length) {
    Int64_t Spaces = 0;
    for (Int64_t Index = 0; Index < BufferLength-1; ++Index) {
        if (Buffer[Index] == ' ') {
            Spaces += 1;
            if (Spaces == 2) {
                *Length = Index;
                return Index;
            }
            
        }

        Value[Index] = Buffer[Index];
    }

    Panic("Could not find a color.");
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day07_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumRules += 1;
        Assert(Day.NumRules <= MAX_RULE_COUNT);
        Rule_t* Rule = &Day.Rules[Day.NumRules - 1];

        Offset += ScanForColor(Input.Data + Offset, MAX_COLOR_LENGTH, Rule->Color, &Rule->ColorLength);

        Offset += 14; // Eat the " bags contain ".

        while (Input.Data[Offset] != '\n') {
            if (StringStartsWith(Input.Data +Offset, "no other bags.")) {
                Offset += 14; // Eat the "no other bags."
                break;
            }

            Rule->NumContents += 1;
            Assert(Rule->NumContents <= MAX_CONTENT_COUNT);
            Content_t* Content = &Rule->Contents[Rule->NumContents - 1];

            Offset += ScanForInt(Input.Data + Offset, &Content->Quantity);

            Offset += 1; // Eat the space.
            Offset += ScanForColor(Input.Data + Offset, MAX_COLOR_LENGTH, Content->Color, &Content->ColorLength);

            while ((Input.Data[Offset] != ',') && (Input.Data[Offset] != '.')) {
                Offset += 1;
            }

            if (Input.Data[Offset] == ',') {
                Offset += 2; // Eat the comma and the space.
            } else if (Input.Data[Offset] == '.') {
                Offset += 1; // Eat the space.
            }
        }

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
