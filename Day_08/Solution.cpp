#include <cstdio>
#include <Advent.h>

struct Instruction_t {
    Bool_t Visited;
    Int32_t Argument;
    Char_t const* Operation;
};

#define MAX_INSTRUCTION_COUNT 1024

struct Day08_t {
    Int32_t Count;
    Instruction_t Instructions[MAX_INSTRUCTION_COUNT];
};

Bool_t RunProgram(Day08_t* Day, Int64_t* Accumulator) {
    Int32_t ProgramCounter = 0;
    while (ProgramCounter < Day->Count) {
        Instruction_t* Instruction =
            &Day->Instructions[ProgramCounter];
            
        if (Instruction->Visited) {
            return false;
        }

        Instruction->Visited = true;
        if (StringEquals(Instruction->Operation, "acc")) {
            *Accumulator += Instruction->Argument;
        } else if (StringEquals(Instruction->Operation, "jmp")) {
            ProgramCounter += Instruction->Argument;
            continue; // Do not increment the program counter below.
        }

        ProgramCounter += 1;
    }

    return true;
}

Int64_t SolvePart1(Day08_t* Day) {
    Int64_t Accumulator = 0;
    if (!RunProgram(Day, &Accumulator)) {
        return Accumulator;
    }

    Panic("No solution found.");
}

void ResetProgram(Day08_t* Day) {
    Int32_t ProgramCounter = 0;
    while (ProgramCounter < Day->Count) {
        Instruction_t* Instruction =
            &Day->Instructions[ProgramCounter];

        Instruction->Visited = false;
        ProgramCounter += 1;
    }
}

Int64_t SolvePart2(Day08_t* Day) {
    for (Int32_t i = 0; i < Day->Count; ++i) {
        Instruction_t* Instruction = &Day->Instructions[i];

        if (StringEquals(Instruction->Operation, "jmp")) {
            Instruction->Operation = "nop";

            Int64_t Accumulator = 0;
            if (RunProgram(Day, &Accumulator)) {
                return Accumulator;
            }

            Instruction->Operation = "jmp";
            ResetProgram(Day);
        } else if (StringEquals(Instruction->Operation, "nop")) {
            Instruction->Operation = "jmp";

            Int64_t Accumulator = 0;
            if (RunProgram(Day, &Accumulator)) {
                return Accumulator;
            }

            Instruction->Operation = "nop";
            ResetProgram(Day);
        }
    }

    Panic("No solution found.");
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day08_t Day08 = {};
    Char_t* Search = &Input.Data[0];
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == '\n') {
            Input.Data[i] = NULL;

            Instruction_t NewInstruction = {};
            for (Char_t* At = Search; *At != NULL; ++At) {
                if (*At == ' ') {
                    *At = NULL;
                    NewInstruction.Operation = Search;

                    Search = At + 1;
                    NewInstruction.Argument = atoi(Search);
                    break;
                }
            }

            Day08.Count += 1;
            Assert(Day08.Count <= MAX_INSTRUCTION_COUNT);
            Day08.Instructions[Day08.Count - 1] = NewInstruction;

            Search = &Input.Data[i + 1];
        }
    }
    
    printf("Part 1: %lld\n", SolvePart1(&Day08));
    printf("Part 2: %lld\n", SolvePart2(&Day08));

    return EXIT_SUCCESS;
}
