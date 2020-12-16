#include <cstdio>
#include <Advent.h>

struct Range_t {
    Int64_t Minimum;
    Int64_t Maximum;
};

#define MAX_NAME_LENGTH 32

struct Rule_t {
    Int64_t NameLength;
    Char_t Name[MAX_NAME_LENGTH];
    
    Range_t RangeA;
    Range_t RangeB;
};

#define MAX_FIELD_COUNT 32

struct Ticket_t {
    Bool_t Valid;
    Int64_t NumFields;
    Int64_t Fields[MAX_FIELD_COUNT];
};

#define MAX_RULE_COUNT 32
#define MAX_TICKET_COUNT 256

struct Day16_t {
    Int64_t NumRules;
    Rule_t Rules[MAX_RULE_COUNT];

    Ticket_t YourTicket;

    Int64_t NumTickets;
    Ticket_t Tickets[MAX_TICKET_COUNT];
};

Bool_t WithinRange(Range_t* Range, Int64_t Value) {
    return (Range->Minimum <= Value) && (Value <= Range->Maximum);
}

Int64_t SolvePart1(Day16_t* Day) {
    Int64_t ErrorRate = 0;
    for (Int64_t TicketIndex = 0; TicketIndex < Day->NumTickets; ++TicketIndex) {
        Ticket_t* Ticket = &Day->Tickets[TicketIndex];

        for (Int64_t FieldIndex = 0; FieldIndex < Ticket->NumFields; ++FieldIndex) {
            Int64_t Value = Ticket->Fields[FieldIndex];

            Bool_t Valid = false;
            for (Int64_t RuleIndex = 0; RuleIndex < Day->NumRules; ++RuleIndex) {
                Rule_t* Rule = &Day->Rules[RuleIndex];
                
                if (WithinRange(&Rule->RangeA, Value) || WithinRange(&Rule->RangeB, Value)) {
                    Valid = true;
                    break;
                }
            }

            if (Valid == false) {
                ErrorRate += Value;
                Ticket->Valid = false;
            }
        }
    }

    return ErrorRate;
}

Int64_t SolvePart2(Day16_t* Day) {
    Bool_t Mapping[MAX_FIELD_COUNT][MAX_RULE_COUNT] = {};
    for (Int64_t i = 0; i < MAX_RULE_COUNT; ++i) {
        for (Int64_t j = 0; j < MAX_FIELD_COUNT; ++j) {
            Mapping[j][i] = true;
        }
    }
    
    for (Int64_t TicketIndex = 0; TicketIndex < Day->NumTickets; ++TicketIndex) {
        Ticket_t* Ticket = &Day->Tickets[TicketIndex];

        if (Ticket->Valid) {
            for (Int64_t RuleIndex = 0; RuleIndex < Day->NumRules; ++RuleIndex) {
                Rule_t* Rule = &Day->Rules[RuleIndex];

                for (Int64_t FieldIndex = 0; FieldIndex < Ticket->NumFields; ++FieldIndex) {
                    Int64_t Value = Ticket->Fields[FieldIndex];

                    if (!WithinRange(&Rule->RangeA, Value) && !WithinRange(&Rule->RangeB, Value)) {
                        Mapping[FieldIndex][RuleIndex] = false;
                    }
                }
            }
        }
    }

    Ticket_t* Ticket = &Day->YourTicket;

    Int64_t Result = 1;
    Int64_t NumFound = 0;
    while (NumFound < Day->NumRules) {
        for (Int64_t RuleIndex = 0; RuleIndex < Day->NumRules; ++RuleIndex) {
            Rule_t* Rule = &Day->Rules[RuleIndex];

            Int64_t NumOptions = 0;
            Int64_t OptionIndex = -1;
            for (Int64_t FieldIndex = 0; FieldIndex < Ticket->NumFields; ++FieldIndex) {
                if (Mapping[FieldIndex][RuleIndex]) {
                    NumOptions += 1;
                    OptionIndex = FieldIndex;
                }
            }

            if (NumOptions == 1) {
                NumFound += 1;

                for (Int64_t i = 0; i < Day->NumRules; ++i) {
                    Mapping[OptionIndex][i] = false;
                }

                if (StringStartsWith(Rule->Name, "departure")) {
                    Result *= Ticket->Fields[OptionIndex];
                }
            }
        }
    }

    return Result;
}

Int64_t ParseRules(Day16_t* Day, Input_t* Input, Int64_t Base) {
    Int64_t Offset = 0;
    while (true) {
        Day->NumRules += 1;
        Assert(Day->NumRules <= MAX_RULE_COUNT);
        Rule_t* Rule = &Day->Rules[Day->NumRules - 1];

        while (Input->Data[Base + Offset] != ':') {
            Rule->NameLength += 1;
            Assert(Rule->NameLength <= MAX_NAME_LENGTH);
            Rule->Name[Rule->NameLength - 1] = Input->Data[Base + Offset];
            Offset += 1;
        }

        Offset += 2; // Eat the colon and the space.

        Offset += ScanForInt(Input->Data + Base + Offset, &Rule->RangeA.Minimum);
        Offset += 1; // Eat the hyphen.

        Offset += ScanForInt(Input->Data + Base + Offset, &Rule->RangeA.Maximum);
        Offset += 4; // Eat the " or ".

        Offset += ScanForInt(Input->Data + Base + Offset, &Rule->RangeB.Minimum);
        Offset += 1; // Eat the hyphen.

        Offset += ScanForInt(Input->Data + Base + Offset, &Rule->RangeB.Maximum);
        Offset += 1; // Eat the newline.

        if (Input->Data[Base + Offset] == '\n') {
            Offset += 1; // Eat the newline.
            return Offset;
        }
    }
}

Int64_t ParseTicket(Day16_t* Day, Input_t* Input, Int64_t Base, Ticket_t* Ticket) {
    Int64_t Offset = 0;
    while (true) {
        Ticket->NumFields += 1;
        Assert(Ticket->NumFields <= MAX_FIELD_COUNT);
        Int64_t* Field = &Ticket->Fields[Ticket->NumFields - 1];

        Offset += ScanForInt(Input->Data + Base + Offset, Field);

        if (Input->Data[Base + Offset] == ',') {
            Offset += 1; // Eat the comma.
        } else if (Input->Data[Base + Offset] == '\n') {
            Offset += 1; // Eat the newline.
            return Offset;
        }
    }
}

Int64_t ParseYourTicket(Day16_t* Day, Input_t* Input, Int64_t Base) {
    Int64_t Offset = 13; // Eat the "your ticket:\n".

    Ticket_t* Ticket = &Day->YourTicket;
    Offset += ParseTicket(Day, Input, Base + Offset, Ticket);
    Offset += 1; // Eat the newline.
    
    return Offset;
}

Int64_t ParseNearbyTickets(Day16_t* Day, Input_t* Input, Int64_t Base) {
    Int64_t Offset = 16; // Eat the "nearby tickets:\n".
    while (true) {
        Day->NumTickets += 1;
        Assert(Day->NumTickets <= MAX_TICKET_COUNT);
        Ticket_t* Ticket = &Day->Tickets[Day->NumTickets - 1];

        Ticket->Valid = true; // Potentially.

        Offset += ParseTicket(Day, Input, Base + Offset, Ticket);
        
        if ((Base + Offset) == Input->Length) {
            return Offset;
        }
    }
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day16_t Day = {};
    Int64_t Offset = 0;
    Offset += ParseRules(&Day, &Input, Offset);
    Offset += ParseYourTicket(&Day, &Input, Offset);
    Offset += ParseNearbyTickets(&Day, &Input, Offset);

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
