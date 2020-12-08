#include <cstdio>
#include <Advent.h>

#define MAX_OPERATION_LENGTH 8

struct Instruction_t {
    Bool_t Visited;
    Int64_t Argument;

    Int64_t OperationLength;
    Char_t Operation[MAX_OPERATION_LENGTH];
};

#define MAX_INSTRUCTION_COUNT 1024

struct Day08_t {
    Int64_t NumInstructions;
    Instruction_t Instructions[MAX_INSTRUCTION_COUNT];
};

Bool_t RunProgram(Day08_t* Day, Int64_t* Accumulator) {
    Int64_t ProgramCounter = 0;
    while (ProgramCounter < Day->NumInstructions) {
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
    Int64_t ProgramCounter = 0;
    while (ProgramCounter < Day->NumInstructions) {
        Instruction_t* Instruction =
            &Day->Instructions[ProgramCounter];

        Instruction->Visited = false;
        ProgramCounter += 1;
    }
}

Int64_t SolvePart2(Day08_t* Day) {
    for (Int64_t i = 0; i < Day->NumInstructions; ++i) {
        Instruction_t* Instruction = &Day->Instructions[i];

        if (StringEquals(Instruction->Operation, "jmp")) {
            StringCopy(Instruction->Operation, "nop");

            Int64_t Accumulator = 0;
            if (RunProgram(Day, &Accumulator)) {
                return Accumulator;
            }

            StringCopy(Instruction->Operation, "jmp");
            ResetProgram(Day);
        } else if (StringEquals(Instruction->Operation, "nop")) {
            StringCopy(Instruction->Operation, "jmp");

            Int64_t Accumulator = 0;
            if (RunProgram(Day, &Accumulator)) {
                return Accumulator;
            }

            StringCopy(Instruction->Operation, "nop");
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

    Day08_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumInstructions += 1;
        Assert(Day.NumInstructions <= MAX_INSTRUCTION_COUNT);
        Instruction_t* Instruction = &Day.Instructions[Day.NumInstructions - 1];

        Offset += ScanForString(Input.Data + Offset, MAX_OPERATION_LENGTH, Instruction->Operation, &Instruction->OperationLength);

        Offset += 1; // Eat the space.
        Offset += ScanForInt(Input.Data + Offset, &Instruction->Argument);

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
