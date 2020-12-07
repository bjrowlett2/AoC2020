#include <cstdio>
#include <Advent.h>

struct Content_t {
    Char_t* Color;
    Int32_t Quantity;
};

#define MAX_CONTENT_COUNT 8

struct Rule_t {
    Char_t* Color;

    Int32_t Count;
    Content_t Contents[MAX_CONTENT_COUNT];
};

#define MAX_RULE_COUNT 1024

struct Day07_t {
    Int32_t Count;
    Rule_t Rules[MAX_RULE_COUNT];
};

Bool_t FindRule(Day07_t* Day, Rule_t** Result, Char_t const* Color) {
    for (Int32_t RuleIndex = 0; RuleIndex < Day->Count; ++RuleIndex) {
        Rule_t* Rule = &Day->Rules[RuleIndex];

        if (StringEquals(Color, Rule->Color)) {
            *Result = Rule;
            return true;
        }
    }

    return false;
}

Bool_t CanContainBag(Day07_t* Day, Rule_t* Rule, Char_t const* Color) {
    for (Int32_t ContentIndex = 0; ContentIndex < Rule->Count; ++ContentIndex) {
        Content_t* Content = &Rule->Contents[ContentIndex];

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
    for (Int32_t RuleIndex = 0; RuleIndex < Day->Count; ++RuleIndex) {
        Rule_t* Rule = &Day->Rules[RuleIndex];

        if (CanContainBag(Day, Rule, "shiny gold")) {
            NumBags += 1;
        }
    }

    return NumBags;
}

Int64_t CountNestedBags(Day07_t* Day, Rule_t* Rule) {
    Int64_t Count = 1;
    for (Int32_t ContentIndex = 0; ContentIndex < Rule->Count; ++ContentIndex) {
        Content_t* Content = &Rule->Contents[ContentIndex];

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

Char_t* ReadColor(Char_t** Line) {
    Int32_t Spaces = 0;
    Char_t* Search = *Line;
    for (Char_t* At = Search; *At != NULL; ++At) {
        if (*At == ' ') {
            Spaces += 1;
            if (Spaces == 2) {
                *At = NULL;
                *Line = At + 1;
                return Search;
            }
        }
    }

    Panic("No color found.");
}

Int32_t ReadQuantity(Char_t** Line) {
    Char_t* Start = *Line;
    Char_t* Search = *Line;
    while (IsDigit(*Search)) {
        Search += 1;
    }

    *Search = NULL;
    *Line = Search + 1;
    return atoi(Start);
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day07_t Day07 = {};
    Char_t* Search = &Input.Data[0];
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            Input.Data[i] = NULL;

            Rule_t NewRule = {};
            NewRule.Color = ReadColor(&Search);
            Search += StringLength("bags contain ");

            while (*Search != NULL) {
                if (StringStartsWith(Search, "no other bags.")) {
                    Search += StringLength("no other bags.");
                } else {
                    Content_t NewContent = {};
                    NewContent.Quantity = ReadQuantity(&Search);
                    NewContent.Color = ReadColor(&Search);

                    NewRule.Count += 1;
                    Assert(NewRule.Count <= MAX_CONTENT_COUNT);
                    NewRule.Contents[NewRule.Count - 1] = NewContent;

                    while ((*Search != ',') && (*Search != '.')) {
                        Search += 1;
                    }

                    Search += 1;
                    if (*Search == ' ') {
                        Search += 1;
                    }
                }
            }

            Day07.Count += 1;
            Assert(Day07.Count <= MAX_RULE_COUNT);
            Day07.Rules[Day07.Count - 1] = NewRule;

            Search = &Input.Data[i + 1];
        }
    }
    
    printf("Part 1: %lld\n", SolvePart1(&Day07));
    printf("Part 2: %lld\n", SolvePart2(&Day07));

    return EXIT_SUCCESS;
}
