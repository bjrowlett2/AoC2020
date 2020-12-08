#include <cstdio>
#include <Advent.h>

#define MAX_NAME_LENGTH 8
#define MAX_VALUE_LENGTH 16

struct Field_t {
    Int64_t NameLength;
    Char_t Name[MAX_NAME_LENGTH];

    Int64_t ValueLength;
    Char_t Value[MAX_VALUE_LENGTH];
};

#define MAX_FIELD_COUNT 8

struct Passport_t {
    Int64_t NumFields;
    Field_t Fields[MAX_FIELD_COUNT];
};

#define MAX_PASSPORT_COUNT 512

struct Day04_t {
    Int64_t NumPassports;
    Passport_t Passports[MAX_PASSPORT_COUNT];
};

#define BYR_BIT 0x01
#define IYR_BIT 0x02
#define EYR_BIT 0x04
#define HGT_BIT 0x08
#define HCL_BIT 0x10
#define ECL_BIT 0x20
#define PID_BIT 0x40
#define REQUIRED_BIT_MASK 0x7F

Int64_t SolvePart1(Day04_t* Day) {
    Int64_t ValidPassports = 0;
    for (Int64_t i = 0; i < Day->NumPassports; ++i) {
        Passport_t* Passport = &Day->Passports[i];

        Int64_t FieldMask = 0;
        for (Int64_t j = 0; j < Passport->NumFields; ++j) {
            Field_t* Field = &Passport->Fields[j];

            if (StringEquals(Field->Name, "byr")) {
                FieldMask |= BYR_BIT;
            } else if (StringEquals(Field->Name, "iyr")) {
                FieldMask |= IYR_BIT;
            } else if (StringEquals(Field->Name, "eyr")) {
                FieldMask |= EYR_BIT;
            } else if (StringEquals(Field->Name, "hgt")) {
                FieldMask |= HGT_BIT;
            } else if (StringEquals(Field->Name, "hcl")) {
                FieldMask |= HCL_BIT;
            } else if (StringEquals(Field->Name, "ecl")) {
                FieldMask |= ECL_BIT;
            } else if (StringEquals(Field->Name, "pid")) {
                FieldMask |= PID_BIT;
            }
        }

        if (FieldMask == REQUIRED_BIT_MASK) {
            ValidPassports += 1;
        }
    }

    return ValidPassports;
}

#define BYR_LENGTH 4
#define IYR_LENGTH 4
#define EYR_LENGTH 4
#define HCL_LENGTH 7
#define PID_LENGTH 9

Int64_t SolvePart2(Day04_t* Day) {
    Int64_t ValidPassports = 0;
    for (Int64_t i = 0; i < Day->NumPassports; ++i) {
        Passport_t* Passport = &Day->Passports[i];

        Int64_t FieldMask = 0;
        for (Int64_t j = 0; j < Passport->NumFields; ++j) {
            Field_t* Field = &Passport->Fields[j];

            if (StringEquals(Field->Name, "byr")) {
                if (StringLength(Field->Value) == BYR_LENGTH) {
                    Int64_t Year = atoi(Field->Value);
                    if ((1920 <= Year) && (Year <= 2002)) {
                        FieldMask |= BYR_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "iyr")) {
                if (StringLength(Field->Value) == IYR_LENGTH) {
                    Int64_t Year = atoi(Field->Value);
                    if ((2010 <= Year) && (Year <= 2020)) {
                        FieldMask |= IYR_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "eyr")) {
                if (StringLength(Field->Value) == EYR_LENGTH) {
                    Int64_t Year = atoi(Field->Value);
                    if ((2020 <= Year) && (Year <= 2030)) {
                        FieldMask |= EYR_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "hgt")) {
                Int64_t Length = 0;
                while (IsDigit(Field->Value[Length])) {
                    Length += 1;
                }

                Char_t* Units = &Field->Value[Length];
                if (StringEquals(Units, "cm")) {
                    Field->Value[Length] = NULL; // @Cleanup

                    Int64_t Height = atoi(Field->Value);
                    if ((150 <= Height) && (Height <= 193)) {
                        FieldMask |= HGT_BIT;
                    }
                } else if (StringEquals(Units, "in")) {
                    Field->Value[Length] = NULL; // @Cleanup

                    Int64_t Height = atoi(Field->Value);
                    if ((59 <= Height) && (Height <= 76)) {
                        FieldMask |= HGT_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "hcl")) {
                if (StringLength(Field->Value) == HCL_LENGTH) {
                    if (Field->Value[0] == '#') {
                        FieldMask |= HCL_BIT;
                        for (Int64_t z = 1; z < HCL_LENGTH; ++z) {
                            if (!IsHexDigit(Field->Value[z])) {
                                FieldMask &= ~HCL_BIT;
                                break;
                            }
                        }
                    }
                }
            } else if (StringEquals(Field->Name, "ecl")) {
                if (StringEquals(Field->Value, "amb")) {
                    FieldMask |= ECL_BIT;
                } else if (StringEquals(Field->Value, "blu")) {
                    FieldMask |= ECL_BIT;
                } else if (StringEquals(Field->Value, "brn")) {
                    FieldMask |= ECL_BIT;
                } else if (StringEquals(Field->Value, "gry")) {
                    FieldMask |= ECL_BIT;
                } else if (StringEquals(Field->Value, "grn")) {
                    FieldMask |= ECL_BIT;
                } else if (StringEquals(Field->Value, "hzl")) {
                    FieldMask |= ECL_BIT;
                } else if (StringEquals(Field->Value, "oth")) {
                    FieldMask |= ECL_BIT;
                }
            } else if (StringEquals(Field->Name, "pid")) {
                if (StringLength(Field->Value) == PID_LENGTH) {
                    FieldMask |= PID_BIT;
                    for (Int64_t z = 0; z < PID_LENGTH; ++z) {
                        if (!IsDigit(Field->Value[z])) {
                            FieldMask &= ~PID_BIT;
                            break;
                        }
                    }
                }
            }
        }

        if (FieldMask == REQUIRED_BIT_MASK) {
            ValidPassports += 1;
        }
    }

    return ValidPassports;
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day04_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        if (Offset == 0) {
            Day.NumPassports += 1;
            Assert(Day.NumPassports <= MAX_PASSPORT_COUNT);
        } else if (Input.Data[Offset] == '\n') {
            Day.NumPassports += 1;
            Assert(Day.NumPassports <= MAX_PASSPORT_COUNT);

            Offset += 1; // Eat the newline.
        }

        Passport_t* Passport = &Day.Passports[Day.NumPassports - 1];

        Passport->NumFields += 1;
        Assert(Passport->NumFields <=MAX_FIELD_COUNT);
        Field_t* Field = &Passport->Fields[Passport->NumFields - 1];

        for (Int64_t i = 0; i < MAX_NAME_LENGTH; ++i, ++Offset) {
            if (Input.Data[Offset] == ':') {
                break;
            }
            
            Field->NameLength += 1;
            Field->Name[i] = Input.Data[Offset];
        }

        Offset += 1; // Eat the colon.
        Offset += ScanForString(Input.Data + Offset, MAX_VALUE_LENGTH, Field->Value, &Field->ValueLength);

        Offset += 1; // Eat the space or the newline.
    }

    printf("Part 1: %lld\n", SolvePart1(&Day));
    printf("Part 2: %lld\n", SolvePart2(&Day));

    return EXIT_SUCCESS;
}

