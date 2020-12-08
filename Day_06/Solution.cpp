#include <cstdio>
#include <Advent.h>

#define MAX_ANSWER_LENGTH 32

struct Member_t {
    Char_t Answers[MAX_ANSWER_LENGTH];
};

#define MAX_MEMBER_COUNT 16

struct Group_t {
    Int64_t NumMembers;
    Member_t Members[MAX_MEMBER_COUNT];
};

#define MAX_GROUP_COUNT 1024

struct Day06_t {
    Int64_t NumGroups;
    Group_t Groups[MAX_GROUP_COUNT];
};

#define QUESTION_COUNT 26

Int64_t SolvePart1(Day06_t* Day) {
    Int64_t TotalCount = 0;
    for (Int64_t i = 0; i < Day->NumGroups; ++i) {
        Group_t* Group = &Day->Groups[i];

        Bool_t Questions[QUESTION_COUNT] = {};
        for (Int64_t j = 0; j < Group->NumMembers; ++j) {
            Member_t* Member = &Group->Members[j];

            for (Char_t* At = Member->Answers; *At != NULL; ++At) {
                Questions[*At - 'a'] = true;
            }
        }

        for (Int64_t j = 0; j < QUESTION_COUNT; ++j) {
            if (Questions[j]) {
                TotalCount += 1;
            }
        }
    }

    return TotalCount;
}

Int64_t SolvePart2(Day06_t* Day) {
    Int64_t TotalCount = 0;
    for (Int64_t i = 0; i < Day->NumGroups; ++i) {
        Group_t* Group = &Day->Groups[i];

        Bool_t Questions[QUESTION_COUNT] = {};
        Member_t* FirstMember = &Group->Members[0];
        for (Char_t* At = FirstMember->Answers; *At; ++At) {
            Questions[*At - 'a'] = true;
        }
        
        for (Int64_t j = 1; j < Group->NumMembers; ++j) {
            Member_t* Member = &Group->Members[j];

            for (Int64_t k = 0; k < QUESTION_COUNT; ++k) {
                Bool_t Found = false;
                for (Char_t* At = Member->Answers; *At != NULL; ++At) {
                    if ((*At - 'a') == k) {
                        Found = true;
                    }
                }

                if (!Found) {
                    Questions[k] = false;
                }
            }
        }

        for (Int64_t j = 0; j < QUESTION_COUNT; ++j) {
            if (Questions[j]) {
                TotalCount += 1;
            }
        }
    }

    return TotalCount;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day06_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        if (Offset == 0) {
            Day.NumGroups += 1;
            Assert(Day.NumGroups <= MAX_GROUP_COUNT);
        } else if (Input.Data[Offset] == '\n') {
            Day.NumGroups += 1;
            Assert(Day.NumGroups <= MAX_GROUP_COUNT);

            Offset += 1; // Eat the newline.
        }

        Group_t* Group = &Day.Groups[Day.NumGroups - 1];

        Group->NumMembers += 1;
        Assert(Group->NumMembers <= MAX_MEMBER_COUNT);
        Member_t* Member = &Group->Members[Group->NumMembers - 1];

        Int64_t AnswerLength = 0;
        Offset += ScanForString(Input.Data + Offset, MAX_ANSWER_LENGTH, Member->Answers, &AnswerLength);

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
