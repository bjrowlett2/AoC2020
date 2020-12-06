#include <cstdio>
#include <Advent.h>

struct Member_t {
    Char_t* Answers;
};

#define MAX_MEMBER_COUNT 16

struct Group_t {
    Int32_t Count;
    Member_t Members[MAX_MEMBER_COUNT];
};

#define MAX_GROUP_COUNT 1024

struct Day06_t {
    Int32_t Count;
    Group_t Groups[MAX_GROUP_COUNT];
};

#define QUESTION_COUNT 26

Int64_t SolvePart1(Day06_t* Day) {
    Int64_t TotalCount = 0;
    for (Int32_t GroupIndex = 0; GroupIndex < Day->Count; ++GroupIndex) {
        Group_t* Group = &Day->Groups[GroupIndex];

        Bool_t Questions[QUESTION_COUNT] = {};
        for (Int32_t MemberIndex = 0; MemberIndex < Group->Count; ++MemberIndex) {
            Member_t* Member = &Group->Members[MemberIndex];

            for (Char_t* At = Member->Answers; *At != NULL; ++At) {
                Questions[*At - 'a'] = true;
            }
        }

        for (Int32_t QuestionIndex = 0; QuestionIndex < QUESTION_COUNT; ++QuestionIndex) {
            if (Questions[QuestionIndex]) {
                TotalCount += 1;
            }
        }
    }

    return TotalCount;
}

Int64_t SolvePart2(Day06_t* Day) {
    Int64_t TotalCount = 0;
    for (Int32_t GroupIndex = 0; GroupIndex < Day->Count; ++GroupIndex) {
        Group_t* Group = &Day->Groups[GroupIndex];

        Bool_t Questions[QUESTION_COUNT] = {};
        Member_t* FirstMember = &Group->Members[0];
        for (Char_t* At = FirstMember->Answers; *At; ++At) {
            Questions[*At - 'a'] = true;
        }
        
        for (Int32_t MemberIndex = 1; MemberIndex < Group->Count; ++MemberIndex) {
            Member_t* Member = &Group->Members[MemberIndex];

            for (Int32_t QuestionIndex = 0; QuestionIndex < QUESTION_COUNT; ++QuestionIndex) {
                Bool_t Found = false;
                for (Char_t* At = Member->Answers; *At != NULL; ++At) {
                    if ((*At - 'a') == QuestionIndex) {
                        Found = true;
                    }
                }

                if (!Found) {
                    Questions[QuestionIndex] = false;
                }
            }
        }

        for (Int32_t QuestionIndex = 0; QuestionIndex < QUESTION_COUNT; ++QuestionIndex) {
            if (Questions[QuestionIndex]) {
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

    Day06_t Day06 = {};
    Group_t NewGroup = {};
    Member_t NewMember = {};
    Char_t* Search = &Input.Data[0];
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            Input.Data[i] = NULL;

            NewMember.Answers = Search;

            NewGroup.Count += 1;
            Assert(NewGroup.Count <= MAX_MEMBER_COUNT);
            NewGroup.Members[NewGroup.Count - 1] = NewMember;
            Search = &Input.Data[i + 1];

            if ((i == (Input.Length - 1)) || (Input.Data[i + 1] == '\n')) {
                i += 1;
                Search = &Input.Data[i + 1];

                Day06.Count += 1;
                Assert(Day06.Count <= MAX_GROUP_COUNT);
                Day06.Groups[Day06.Count - 1] = NewGroup;

                NewGroup = {};
                NewMember = {};
            }
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day06));
    printf("Part 2: %lld\n", SolvePart2(&Day06));

    return EXIT_SUCCESS;
}
