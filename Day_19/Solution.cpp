#include <cstdio>
#include <Advent.h>

enum Kind_t {
    TERMINAL,
    EXPRESSION
};

struct TerminalRule_t {
    Char_t Letter;
};

#define MAX_ELEMENT_COUNT 4

struct Variation_t {
    Int64_t NumElements;
    Int64_t Elements[MAX_ELEMENT_COUNT];
};

#define MAX_VARIATION_COUNT 4

struct ExpressionRule_t {
    Int64_t NumVariations;
    Variation_t Variations[MAX_VARIATION_COUNT];
};

struct Rule_t {
    Kind_t Kind;
    TerminalRule_t Terminal;
    ExpressionRule_t Expression;
};

#define MAX_DATA_LENGTH 128

struct Message_t {
    Int64_t Length;
    Char_t Data[MAX_DATA_LENGTH];
};

#define MAX_RULE_COUNT 256
#define MAX_MESSAGE_COUNT 512

struct Day19_t {
    Int64_t NumRules;
    Rule_t Rules[MAX_RULE_COUNT];

    Int64_t NumMessages;
    Message_t Messages[MAX_MESSAGE_COUNT];
};

Bool_t MatchesRule(Day19_t* Day, Rule_t* Rule, Message_t* Message, Int64_t Base, Int64_t* Offset) {
    if ((Base + *Offset) >= Message->Length) {
        return false;
    }

    if (Rule->Kind == TERMINAL) {
        TerminalRule_t* Terminal = &Rule->Terminal;
        if (Message->Data[Base + *Offset] == Terminal->Letter) {
            *Offset += 1;
            return true;
        }
    } else if (Rule->Kind == EXPRESSION) {
        ExpressionRule_t* Expression = &Rule->Expression;
        for (Int64_t i = 0; i < Expression->NumVariations; ++i) {
            Variation_t* Variation = &Expression->Variations[i];

            Bool_t Success = true;
            Int64_t NewOffset = 0;
            for (Int64_t j = 0; j < Variation->NumElements; ++j) {
                Int64_t Element = Variation->Elements[j];
                Rule_t* NestedRule = &Day->Rules[Element];

                Int64_t NewBase = Base + *Offset;
                if (!MatchesRule(Day, NestedRule, Message, NewBase, &NewOffset)) {
                    Success = false;
                }
            }

            if (Success) {
                *Offset += NewOffset;
                return true;
            }
        }
    }

    return false;
}

Int64_t SolvePart1(Day19_t* Day) {
    Int64_t Total = 0;
    for (Int64_t i = 0; i < Day->NumMessages; ++i) {
        Message_t* Message = &Day->Messages[i];

        Int64_t Consumed = 0;
        if (MatchesRule(Day, &Day->Rules[0], Message, 0, &Consumed)) {
            if (Consumed == Message->Length) {
                Total += 1;
            }
        }
    }

    return Total;
}

Bool_t MatchesRule8(Day19_t* Day, Message_t* Message, Int64_t Base, Int64_t* Offset, Int64_t Count) {
    Bool_t Success = true;
    for (Int64_t i = 0; i < Count; ++i) {
        Int64_t Consumed = 0;
        if (!MatchesRule(Day, &Day->Rules[42], Message, Base, &Consumed)) {
            Success = false;
            break;
        }

        Base += Consumed;
        *Offset += Consumed;
    }

    return Success;
}

Bool_t MatchesRule11(Day19_t* Day, Message_t* Message, Int64_t Base, Int64_t* Offset, Int64_t Count) {
    Bool_t Success = true;
    for (Int64_t i = 0; i < Count; ++i) {
        Int64_t Consumed = 0;
        if (!MatchesRule(Day, &Day->Rules[42], Message, Base, &Consumed)) {
            Success = false;
            break;
        }

        Base += Consumed;
        *Offset += Consumed;
    }

    for (Int64_t i = 0; i < Count; ++i) {
        Int64_t Consumed = 0;
        if (!MatchesRule(Day, &Day->Rules[31], Message, Base, &Consumed)) {
            Success = false;
            break;
        }

        Base += Consumed;
        *Offset += Consumed;
    }

    return Success;
}

// Tuned for my input file!
#define MAX_RECURSION_DEPTH 6

Int64_t SolvePart2(Day19_t* Day) {
    // [ 0]: 8 11
    // [ 8]: 42 | 42 8
    // [11]: 42 31 | 42 11 31

    Int64_t Total = 0;
    for (Int64_t i = 0; i < Day->NumMessages; ++i) {
        Message_t* Message = &Day->Messages[i];

        for (Int64_t m = 1; m < MAX_RECURSION_DEPTH; ++m) {
            Int64_t Base = 0;
            Int64_t Offset = 0;
            if (MatchesRule8(Day, Message, Base, &Offset, m)) {
                Base += Offset;
                
                for (Int64_t n = 1; n < MAX_RECURSION_DEPTH; ++n) {
                    Offset = 0;
                    if (MatchesRule11(Day, Message, Base, &Offset, n)) {
                        if ((Base + Offset) == Message->Length) {
                            Total += 1;
                        }
                    }
                }
            }
        }
    }

    return Total;
}

Int64_t ParseRules(Day19_t* Day, Input_t* Input, Int64_t Base) {
    Int64_t Offset = 0;
    while (Input->Data[Base + Offset] != '\n') {
        Day->NumRules +=1;
        Assert(Day->NumRules <= MAX_RULE_COUNT);
        
        Int64_t RuleIndex = 0;
        Offset += ScanForInt(Input->Data + Base + Offset, &RuleIndex);
        Offset += 2; // Eat the colon and the space.

        Rule_t* Rule = &Day->Rules[RuleIndex];

        if (Input->Data[Base + Offset] == '"') {
            Offset += 1; // Eat the double quote.

            Rule->Kind = TERMINAL;

            TerminalRule_t* Terminal = &Rule->Terminal;
            Terminal->Letter = Input->Data[Base + Offset];

            Offset += 2; // Eat the letter and the double quote.
        } else {
            Rule->Kind = EXPRESSION;

            ExpressionRule_t* Expression = &Rule->Expression;

            while (Input->Data[Base + Offset] != '\n') {
                Expression->NumVariations += 1;
                Assert(Expression->NumVariations <= MAX_VARIATION_COUNT);
                Variation_t* Variation = &Expression->Variations[Expression->NumVariations - 1];

                while ((Input->Data[Base + Offset] != '|')
                    && (Input->Data[Base + Offset] != '\n')) {
                        Variation->NumElements += 1;
                        Assert(Variation->NumElements <= MAX_ELEMENT_COUNT);
                        Int64_t* Element = &Variation->Elements[Variation->NumElements - 1];

                        Offset += ScanForInt(Input->Data + Base + Offset, Element);

                        if (Input->Data[Base + Offset] == ' ') {
                            Offset += 1; // Eat the space.
                        }
                }

                if (Input->Data[Base + Offset] == '|') {
                    Offset += 2; // Eat the pipe and the space.
                }
            }
        }

        Offset += 1; // Eat the newline.
    }

    Offset += 1; // Eat the newline.

    return Offset;
}

Int64_t ParseMessages(Day19_t* Day, Input_t* Input, Int64_t Base) {
    Int64_t Offset = 0;
    while ((Base + Offset) < Input->Length) {
        Day->NumMessages += 1;
        Assert(Day->NumMessages <= MAX_MESSAGE_COUNT);
        Message_t* Message = &Day->Messages[Day->NumMessages - 1];

        while (Input->Data[Base + Offset] != '\n') {
            Message->Length += 1;
            Assert(Message->Length <= MAX_DATA_LENGTH);
            Message->Data[Message->Length - 1] = Input->Data[Base + Offset];

            Offset += 1; // Eat the character.
        }

        Offset += 1; // Eat the newline.
    }

    return Offset;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day19_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Offset += ParseRules(&Day, &Input, Offset);
        Offset += ParseMessages(&Day, &Input, Offset);
        break;
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
