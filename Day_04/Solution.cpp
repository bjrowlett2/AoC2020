#include <cstdio>
#include <Advent.h>

struct Field_t {
    Char_t* Name;
    Char_t* Value;
};

#define MAX_FIELD_COUNT 8

struct Passport_t {
    Int32_t Count;
    Field_t Fields[MAX_FIELD_COUNT];
};

#define MAX_PASSPORT_COUNT 512

struct Day04_t {
    Int32_t Count;
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
    for (Int32_t i = 0; i < Day->Count; ++i) {
        Passport_t* Passport = &Day->Passports[i];

        Int32_t FieldMask = 0;
        for (Int32_t j = 0; j < Passport->Count; ++j) {
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
    for (Int32_t i = 0; i < Day->Count; ++i) {
        Passport_t* Passport = &Day->Passports[i];

        Int32_t FieldMask = 0;
        for (Int32_t j = 0; j < Passport->Count; ++j) {
            Field_t* Field = &Passport->Fields[j];

            if (StringEquals(Field->Name, "byr")) {
                if (StringLength(Field->Value) == BYR_LENGTH) {
                    Int32_t Year = atoi(Field->Value);
                    if ((1920 <= Year) && (Year <= 2002)) {
                        FieldMask |= BYR_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "iyr")) {
                if (StringLength(Field->Value) == IYR_LENGTH) {
                    Int32_t Year = atoi(Field->Value);
                    if ((2010 <= Year) && (Year <= 2020)) {
                        FieldMask |= IYR_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "eyr")) {
                if (StringLength(Field->Value) == EYR_LENGTH) {
                    Int32_t Year = atoi(Field->Value);
                    if ((2020 <= Year) && (Year <= 2030)) {
                        FieldMask |= EYR_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "hgt")) {
                Int32_t Length = 0;
                while (IsDigit(Field->Value[Length])) {
                    Length += 1;
                }

                Char_t* Units = &Field->Value[Length];
                if (StringEquals(Units, "cm")) {
                    Field->Value[Length] = NULL; // @Cleanup

                    Int32_t Height = atoi(Field->Value);
                    if ((150 <= Height) && (Height <= 193)) {
                        FieldMask |= HGT_BIT;
                    }
                } else if (StringEquals(Units, "in")) {
                    Field->Value[Length] = NULL; // @Cleanup

                    Int32_t Height = atoi(Field->Value);
                    if ((59 <= Height) && (Height <= 76)) {
                        FieldMask |= HGT_BIT;
                    }
                }
            } else if (StringEquals(Field->Name, "hcl")) {
                if (StringLength(Field->Value) == HCL_LENGTH) {
                    if (Field->Value[0] == '#') {
                        FieldMask |= HCL_BIT;
                        for (Int32_t z = 1; z < HCL_LENGTH; ++z) {
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
                    for (Int32_t z = 0; z < PID_LENGTH; ++z) {
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

    Day04_t Day04 = {};
    Field_t NewField = {};
    Passport_t NewPassport = {};
    Char_t* Search = &Input.Data[0];
    for (Int32_t i = 0; i < Input.Length; ++i) {
        if (Input.Data[i] == ':') {
            Input.Data[i] = NULL;

            NewField.Name = Search;
            Search = &Input.Data[i + 1];
        } else if ((Input.Data[i] == ' ') || (Input.Data[i] == '\n')) {
            Input.Data[i] = NULL;

            NewField.Value = Search;
            Search = &Input.Data[i + 1];

            NewPassport.Count += 1;
            Assert(NewPassport.Count <= MAX_FIELD_COUNT);
            NewPassport.Fields[NewPassport.Count - 1] = NewField;

            if ((i == (Input.Length - 1)) || (Input.Data[i + 1] == '\n')) {
                i += 1;
                Search = &Input.Data[i + 1];

                Day04.Count += 1;
                Assert(Day04.Count <= MAX_PASSPORT_COUNT);
                Day04.Passports[Day04.Count - 1] = NewPassport;

                NewField = {};
                NewPassport = {};
            }
        }
    }

    printf("Part 1: %lld\n", SolvePart1(&Day04));
    printf("Part 2: %lld\n", SolvePart2(&Day04));

    return EXIT_SUCCESS;
}

