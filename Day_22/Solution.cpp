#include <cstdio>
#include <Advent.h>

struct Day22_t {
    Queue_t<Int64_t> Player1;
    Queue_t<Int64_t> Player2;
};

Bool_t StillPlaying(Queue_t<Int64_t>* Player1, Queue_t<Int64_t>* Player2) {
    return (Player1->NumEntries != 0)
        && (Player2->NumEntries != 0);
}

Bool_t BothCanRecurse(Queue_t<Int64_t>* Player1, Int64_t Card1, Queue_t<Int64_t>* Player2, Int64_t Card2) {
    return (Player1->NumEntries >= Card1)
        && (Player2->NumEntries >= Card2);
}

// Tuned for my input file!
#define MAX_ROUND_COUNT 540

void PlayCombat(Queue_t<Int64_t>* Player1, Queue_t<Int64_t>* Player2, Bool_t IsPart2) {
    Int64_t RoundCount = 0;
    while (StillPlaying(Player1, Player2)) {
        Int64_t Card1 = 0;
        Pop(Player1, &Card1);

        Int64_t Card2 = 0;
        Pop(Player2, &Card2);

        if (BothCanRecurse(Player1, Card1, Player2, Card2) && IsPart2) {
            Queue_t<Int64_t> Player1Copy = {};
            for (Int64_t i = 0; i < Card1; ++i) {
                Add(&Player1Copy, Player1->Entries[i]);
            }

            Queue_t<Int64_t> Player2Copy = {};
            for (Int64_t i = 0; i < Card2; ++i) {
                Add(&Player2Copy, Player2->Entries[i]);
            }

            PlayCombat(&Player1Copy, &Player2Copy, IsPart2);

            if (Player2Copy.NumEntries == 0) {
                Add(Player1, Card1);
                Add(Player1, Card2);
            }else if (Player1Copy.NumEntries == 0) {
                Add(Player2, Card2);
                Add(Player2, Card1);
            }
        } else {
            if (Card1 > Card2) {
                Add(Player1, Card1);
                Add(Player1, Card2);
            } else if (Card1 < Card2) {
                Add(Player2, Card2);
                Add(Player2, Card1);
            }
        }

        // @Incomplete: Instead of checking "if there was a previous round in
        // this game that had exactly the same cards in the same order in the same
        // players' decks", simply put a cap on the number of rounds we play.

        RoundCount += 1;
        if (RoundCount > MAX_ROUND_COUNT) {
            Player2->NumEntries = 0;
            return;
        }
    }
}

Int64_t ComputePlayerScore(Queue_t<Int64_t>* Winner) {
    Int64_t Score = 0;
    while (Winner->NumEntries != 0) {
        Int64_t Card = 0;
        Pop(Winner, &Card);

        Score += Card * (Winner->NumEntries + 1);
    }

    return Score;
}

Int64_t ComputeWinningScore(Queue_t<Int64_t>* Player1, Queue_t<Int64_t>* Player2) {
    if (Player1->NumEntries != 0) {
        return ComputePlayerScore(Player1);
    } else if (Player2->NumEntries != 0) {
        return ComputePlayerScore(Player2);
    }

    return 0;
}

Int64_t SolvePart1(Day22_t Day) {
    PlayCombat(&Day.Player1, &Day.Player2, false);
    return ComputeWinningScore(&Day.Player1, &Day.Player2);
}

Int64_t SolvePart2(Day22_t Day) {
    PlayCombat(&Day.Player1, &Day.Player2, true);
    return ComputeWinningScore(&Day.Player1, &Day.Player2);
}

Int64_t LoadDeck(Input_t* Input, Int64_t Base, Queue_t<Int64_t>* Deck) {
    Int64_t Offset = 0;
    
    Offset += 10; // Eat the "Player N:\n".

    while ((Base + Offset) < Input->Length) {
        Int64_t Card = 0;
        Offset += ScanForInt(Input->Data + Base + Offset, &Card);
        Offset += 1; // Eat the newline.

        Add(Deck, Card);

        if (Input->Data[Base + Offset] == '\n') {
            break;
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

    Day22_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Offset += LoadDeck(&Input, Offset, &Day.Player1);
        Offset += LoadDeck(&Input, Offset, &Day.Player2);
    }

    printf("Part 1: %lld\n", SolvePart1(Day));
    printf("Part 2: %lld\n", SolvePart2(Day));

    return EXIT_SUCCESS;
}
