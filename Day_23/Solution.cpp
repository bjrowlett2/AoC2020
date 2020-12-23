#include <cstdio>
#include <Advent.h>

struct LinkedNode_t {
    Int64_t Value;
    LinkedNode_t* Next;
};

#define MAX_NUMBER_COUNT 16

struct Day23_t {
    Int64_t NumNumbers;
    Int64_t Numbers[MAX_NUMBER_COUNT];
};

Bool_t Find(Day23_t* Day, Int64_t Value, Int64_t* Next) {
    *Next = Day->NumNumbers + 1;
    for (Int64_t i = 0; i < Day->NumNumbers; ++i) {
        if (Day->Numbers[i] == Value) {
            if (i != (Day->NumNumbers - 1)) {
                *Next = Day->Numbers[i + 1];
            }

            return true;
        }
    }

    return false;
}

LinkedNode_t* AllocateNodes(Day23_t* Day, Int64_t MaxNodeValue) {
    LinkedNode_t* Nodes = new LinkedNode_t[MaxNodeValue + 1];

    for (Int64_t i = 1; i <= MaxNodeValue; ++i) {
        Int64_t Next = NULL;
        if (Find(Day, i, &Next)) {
            Nodes[i].Value = i;
            if (Next <= MaxNodeValue) {
                Nodes[i].Next = &Nodes[Next];
            } else if (Next > MaxNodeValue) {
                Nodes[i].Next = &Nodes[Day->Numbers[0]];
            }
        } else {
            Nodes[i].Value = i;
            if (i < MaxNodeValue) {
                Nodes[i].Next = &Nodes[i + 1];
            } else if (i == MaxNodeValue) {
                Nodes[i].Next = &Nodes[Day->Numbers[0]];
            }
        }
    }
    
    return Nodes;
}

LinkedNode_t* Cut(LinkedNode_t* Node) {
    LinkedNode_t* Segment = Node->Next;
    Node->Next = Segment->Next->Next->Next;
    Segment->Next->Next->Next = NULL;
    return Segment;
}

void Paste(LinkedNode_t* Node, LinkedNode_t* Segment) {
    Segment->Next->Next->Next = Node->Next;
    Node->Next = Segment;
}

Bool_t Contains(LinkedNode_t* Node, Int64_t Value) {
    while (Node != NULL) {
        if (Node->Value == Value) {
            return true;
        }

        Node = Node->Next;
    }

    return false;
}

LinkedNode_t* SimulateMove(LinkedNode_t* Nodes, Int64_t MaxNodeValue, LinkedNode_t* Current) {
    LinkedNode_t* Segment = Cut(Current);
    Int64_t Destination = Current->Value - 1;
    if (Destination <= 0) {
        Destination = MaxNodeValue;
    }

    while (Contains(Segment, Destination)) {
        Destination -= 1;
        if (Destination <= 0) {
            Destination = MaxNodeValue;
        }
    }

    Paste(&Nodes[Destination], Segment);
    return Current->Next;
}

Int64_t SolvePart1(Day23_t* Day) {
    Int64_t MaxNodeValue = Day->NumNumbers;
    LinkedNode_t* Nodes = AllocateNodes(Day, MaxNodeValue);

    LinkedNode_t* Current = &Nodes[Day->Numbers[0]];
    for (Int64_t i = 0; i < 100; ++i) {
        Current = SimulateMove(Nodes, MaxNodeValue, Current);
    }

    Int64_t Result = 0;
    LinkedNode_t* Cursor = Nodes[1].Next;
    while (Cursor->Value != 1) {
        Result = (Result * 10) + Cursor->Value;
        Cursor = Cursor->Next;
    }

    return Result;
}

Int64_t SolvePart2(Day23_t* Day) {
    Int64_t MaxNodeValue = 1'000'000;
    LinkedNode_t* Nodes = AllocateNodes(Day, MaxNodeValue);

    LinkedNode_t* Current = &Nodes[Day->Numbers[0]];
    for (Int64_t i = 0; i < 10'000'000; ++i) {
        Current = SimulateMove(Nodes, MaxNodeValue, Current);
    }

    return Nodes[1].Next->Value
         * Nodes[1].Next->Next->Value;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day23_t Day = {};
    Int64_t Offset = 0;
    while (Input.Data[Offset] != '\n') {
        Day.NumNumbers += 1;
        Assert(Day.NumNumbers <= MAX_NUMBER_COUNT);
        Day.Numbers[Day.NumNumbers - 1] = Input.Data[Offset] - '0';

        Offset += 1; // Eat the digit.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;

}