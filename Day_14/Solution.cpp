#include <cstdio>
#include <Advent.h>

enum Kind_t {
    KIND_MASK,
    KIND_MEMORY
};

struct MaskInstruction_t {
    Int64_t Ones;
    Int64_t Zeroes;
    Int64_t Exes;
};

struct MemoryInstruction_t {
    Int64_t Address;
    Int64_t Value;
};

struct Instruction_t {
    Kind_t Kind;
    MaskInstruction_t Mask;
    MemoryInstruction_t Memory;
};

#define MAX_INSTRUCTION_COUNT 1024

struct Day14_t {
    Int64_t NumInstructions;
    Instruction_t Instructions[MAX_INSTRUCTION_COUNT];
};

Int64_t SolvePart1(Day14_t* Day) {
    MaskInstruction_t Mask = {};
    HashTable_t<Int64_t, Int64_t>* Memory =
        new HashTable_t<Int64_t, Int64_t>();

    for (Int64_t i = 0; i < Day->NumInstructions; ++i) {
        Instruction_t* Instruction = &Day->Instructions[i];

        if (Instruction->Kind == KIND_MASK) {
            Mask = Instruction->Mask;
        } else if (Instruction->Kind == KIND_MEMORY) {
            Int64_t Value = Mask.Ones | (~Mask.Zeroes & Instruction->Memory.Value);
            Add(Memory, Instruction->Memory.Address, Value);
        }
    }

    Int64_t Total = 0;
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        for (Int64_t j = 0; j < Memory->Buckets[i].NumEntries; ++j) {
            Total += Memory->Buckets[i].Entries[j].Value;
        }
    }

    delete Memory;
    return Total;
}

Bool_t IsBitSet(Int64_t Value, Int64_t Bit) {
    return Value & (1i64 << Bit);
}

Int64_t CountBits(Int64_t Value, Int64_t Size) {
    Int64_t Sum = 0;
    for (Int64_t i = 0; i < Size; ++i) {
        Sum += Value & 1i64;
        Value = Value >> 1i64;
    }

    return Sum;
}

Int64_t SolvePart2(Day14_t* Day) {
    MaskInstruction_t Mask = {};
    HashTable_t<Int64_t, Int64_t>* Memory =
        new HashTable_t<Int64_t, Int64_t>();

    for (Int64_t i = 0; i < Day->NumInstructions; ++i) {
        Instruction_t* Instruction = &Day->Instructions[i];

        if (Instruction->Kind == KIND_MASK) {
            Mask = Instruction->Mask;
        } else if (Instruction->Kind == KIND_MEMORY) {
            Int64_t Permutations = 1i64 << CountBits(Mask.Exes, 36);

            for (Int64_t Config = 0; Config < Permutations; ++Config) {
                Int64_t Address = Mask.Ones | Instruction->Memory.Address;

                ///
                /// Note:
                ///     This is "Expand, or Generalized Insert" from
                ///     the book Hacker's Delight by Henry S. Warren, Jr.
                ///
                ///         expand(0000abcd, 10011010) = a00bc0d0.
                ///
                /// :: Brian Rowlett, 2020-12-14
                ///

                for (Int64_t Shift = 0, n = 0; Shift < 36; ++Shift) {
                    if (IsBitSet(Mask.Exes, Shift)) {
                        Int64_t Target = 1i64 << Shift;
                        Int64_t Expanded = ((Config >> n) & 1i64) << Shift;

                        Address = (Address & ~Target) | Expanded & Target;
                        n += 1;
                    }
                }

                Add(Memory, Address, Instruction->Memory.Value);
            }
        }
    }

    Int64_t Total = 0;
    for (Int64_t i = 0; i < MAX_HASH_BUCKET_COUNT; ++i) {
        for (Int64_t j = 0; j < Memory->Buckets[i].NumEntries; ++j) {
            Total += Memory->Buckets[i].Entries[j].Value;
        }
    }

    delete Memory;
    return Total;
}

#define MAX_OP_LENGTH 8

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day14_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumInstructions += 1;
        Assert(Day.NumInstructions <= MAX_INSTRUCTION_COUNT);
        Instruction_t* Instruction = &Day.Instructions[Day.NumInstructions - 1];

        Int64_t OpLength = 0;
        Char_t Op[MAX_OP_LENGTH] = {};
        Offset += ScanForEnglish(Input.Data + Offset, MAX_OP_LENGTH, Op, &OpLength);

        if (StringEquals(Op, "mask")) {
            Instruction->Kind = KIND_MASK;

            Offset += 3; // Eat the spaces and equals sign.

            for (Int64_t i = 0; i < 36; ++i, ++Offset) {
                Int64_t Shift = 36 - i - 1;
                if (Input.Data[Offset] == '1') {
                    Instruction->Mask.Ones |= (1i64 << Shift);
                } else if (Input.Data[Offset] == '0') {
                    Instruction->Mask.Zeroes |= (1i64 << Shift);
                } else if (Input.Data[Offset] == 'X') {
                    Instruction->Mask.Exes |= (1i64 << Shift);
                }
            }

            Offset += 1; // Eat the newline.
        } else if (StringEquals(Op, "mem")) {
            Instruction->Kind = KIND_MEMORY;

            Offset += 1; // Eat the left bracket.
            Offset += ScanForInt(Input.Data + Offset, &Instruction->Memory.Address);
            Offset += 1; // Eat the right bracket.
            Offset += 3; // Eat the spaces and equals sign.
            Offset += ScanForInt(Input.Data + Offset, &Instruction->Memory.Value);
            Offset += 1; // Eat the newline.
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
