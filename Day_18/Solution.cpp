#include <cstdio>
#include <Advent.h>

enum Kind_t {
    TERMINAL,
    OPERATOR
};

enum Operator_t {
    ADDITION,
    MULTIPLICATION,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS
};

struct Element_t {
    Kind_t Kind;

    union {
        Int64_t Value; // Only for TERMINAL.
        Operator_t Operator; // Only for OPERATOR.
    };
};

#define MAX_SYMBOL_LENGTH 256

struct Expression_t {
    Int64_t NumSymbols;
    Char_t Symbols[MAX_SYMBOL_LENGTH];
};

#define MAX_EXPRESSION_COUNT 512

struct Day18_t {
    Int64_t NumExpressions;
    Expression_t Expressions[MAX_EXPRESSION_COUNT];
};

Queue_t<Element_t> ShuntingYard(Char_t* Expression, Int64_t Precedence[2]) {
    Queue_t<Element_t> Output = {};
    Stack_t<Operator_t> Operators = {};

    Int64_t Offset = 0;
    while (Expression[Offset] != NULL) {
        if (IsDigit(Expression[Offset])) {
            Int64_t Terminal = 0;
            Offset += ScanForInt(Expression + Offset, &Terminal);
            Add(&Output, { TERMINAL, Terminal });
        } else if (IsOperator(Expression[Offset])) {
            Operator_t Next = {};
            switch (Expression[Offset]) {
                case '+':  Next = ADDITION;  break;
                case '*':  Next = MULTIPLICATION;  break;
                case '(':  Next = OPEN_PARENTHESIS;  break;
                case ')':  Next = CLOSE_PARENTHESIS;  break;
                default: Panic("No matching operator found.");
            }

            while (Operators.NumEntries > 0) {
                Operator_t Top = {};
                if (Peek(&Operators, &Top)) {
                    if (Top == OPEN_PARENTHESIS) {
                        break;
                    }

                    if (Precedence[Top] <= Precedence[Next]) {
                        Add(&Output, { OPERATOR, Top });
                        Remove(&Operators);
                        continue;
                    }
                }

                break;
            }

            Add(&Operators, Next);
            Offset += 1;
        } else if (IsOpenParenthesis(Expression[Offset])) {
            Add(&Operators, OPEN_PARENTHESIS);
            Offset += 1;
        } else if (IsCloseParenthesis(Expression[Offset])) {
            while (Operators.NumEntries > 0) {
                Operator_t Top = {};
                if (Peek(&Operators, &Top)) {
                    if (Top == OPEN_PARENTHESIS) {
                        break;
                    }
                }

                Operator_t Operator = {};
                if (Pop(&Operators, &Operator)) {
                    Add(&Output, { OPERATOR, Operator });
                }
            }

            Remove(&Operators); // Discard open parenthesis.
            Offset += 1;
        } else if (IsWhitespace(Expression[Offset])) {
            Offset += 1; // Eat the whitespace.
        }
    }

    while (Operators.NumEntries > 0) {
        Operator_t Operator = {};
        if (Pop(&Operators, &Operator)) {
            Add(&Output, { OPERATOR, Operator });
        }
    }

    return Output;
}

Int64_t Evaluate(Queue_t<Element_t>* Queue) {
    Stack_t<Int64_t> Values = {};
    while (Queue->NumEntries > 0) {
        Element_t Element = {};
        if (Pop(Queue, &Element)) {
            if (Element.Kind == TERMINAL) {
                Add(&Values, Element.Value);
            } else if (Element.Kind == OPERATOR) {
                Int64_t LeftSide = 0;
                Pop(&Values, &LeftSide);

                Int64_t RightSide = 0;
                Pop(&Values, &RightSide);

                if (Element.Operator == ADDITION) {
                    Add(&Values, LeftSide + RightSide);
                } else if (Element.Operator == MULTIPLICATION) {
                    Add(&Values, LeftSide * RightSide);
                }
            }
        }
    }

    Int64_t Result = 0;
    Pop(&Values, &Result);

    return Result;
}

Int64_t SolvePart1(Day18_t* Day) {
    Int64_t Precedence[2] = {};
    Precedence[ADDITION] = 1;
    Precedence[MULTIPLICATION] = 1;

    Int64_t Total = 0;
    for (Int64_t i = 0; i < Day->NumExpressions; ++i) {
        Expression_t* Expression = &Day->Expressions[i];
        Queue_t<Element_t> Output = ShuntingYard(Expression->Symbols, Precedence);

        Total += Evaluate(&Output);
    }

    return Total;
}

Int64_t SolvePart2(Day18_t* Day) {
    Int64_t Precedence[2] = {};
    Precedence[ADDITION] = 1;
    Precedence[MULTIPLICATION] = 2;

    Int64_t Total = 0;
    for (Int64_t i = 0; i < Day->NumExpressions; ++i) {
        Expression_t* Expression = &Day->Expressions[i];
        Queue_t<Element_t> Output = ShuntingYard(Expression->Symbols, Precedence);

        Total += Evaluate(&Output);
    }

    return Total;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day18_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumExpressions += 1;
        Assert(Day.NumExpressions <= MAX_EXPRESSION_COUNT);
        Expression_t* Expression = &Day.Expressions[Day.NumExpressions - 1];

        while (Input.Data[Offset] != '\n') {
            Expression->NumSymbols += 1;
            Assert(Expression->NumSymbols < MAX_SYMBOL_LENGTH); // Leave space for the null terminator.
            Expression->Symbols[Expression->NumSymbols - 1] = Input.Data[Offset++];
        }

        Offset += 1; // Eat the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}
