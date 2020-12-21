#include <cstdio>
#include <Advent.h>

#define MAX_STRING_LENGTH 32

struct String_t {
    Int64_t Length;
    Char_t Name[MAX_STRING_LENGTH];
};

#define MAX_ALLERGEN_COUNT 8
#define MAX_INGREDIENT_COUNT 128

struct Food_t {
    Int64_t NumAllergens;
    String_t Allergens[MAX_ALLERGEN_COUNT];

    Int64_t NumIngredients;
    String_t Ingredients[MAX_INGREDIENT_COUNT];
};

#define MAX_FOOD_COUNT 64

struct Day21_t {
    Int64_t NumFoods;
    Food_t Foods[MAX_FOOD_COUNT];
};

struct AllergenIngredient_t {
    String_t Allergen;
    String_t Ingredient;
};

Bool_t Equals(String_t A, String_t B) {
    return StringEquals(A.Name, B.Name);
}

Int64_t StringCompare(String_t A, String_t B) {
    return StringCompare(A.Name, B.Name);
}

Bool_t Contains(String_t Array[], Int64_t NumEntries, String_t Value) {
    for (Int64_t i = 0; i < NumEntries; ++i) {
        if (StringEquals(Array[i].Name, Value.Name)) {
            return true;
        }
    }

    return false;
}

void FindUniqueAllergens(Day21_t* Day, Set_t<String_t>* UniqueAllergens) {
    for (Int64_t i = 0; i < Day->NumFoods; ++i) {
        Food_t* Food = &Day->Foods[i];

        for (Int64_t j = 0; j < Food->NumAllergens; ++j) {
            Add(UniqueAllergens, Food->Allergens[j]);
        }
    }
}

void FindPossibleIngredients(Day21_t* Day, String_t Allergen, Set_t<String_t>* PossibleIngredients) {
    for (Int64_t i = 0; i < Day->NumFoods; ++i) {
        Food_t* Food = &Day->Foods[i];

        if (Contains(Food->Allergens, Food->NumAllergens, Allergen)) {
            for (Int64_t j = 0; j < Food->NumIngredients; ++j) {
                Add(PossibleIngredients, Food->Ingredients[j]);
            }

            return;
        }
    }
}

void RemoveIncompatibleIngredients(Set_t<String_t>* PossibleIngredients, String_t Ingredients[MAX_FOOD_COUNT], Int64_t NumIngredients) {
    for (Int64_t i = 0; i < PossibleIngredients->NumEntries; ) {
        if (!Contains(Ingredients, NumIngredients, PossibleIngredients->Entries[i])) {
            Remove(PossibleIngredients, i);
        } else {
            ++i;
        }
    }
}

void IdentifyAllergensAndIngredients(Day21_t* Day, Set_t<String_t>* DangerousIngredients, Array_t<AllergenIngredient_t>* AllergenIngredientPairs) {
    Set_t<String_t> UniqueAllergens = {};
    FindUniqueAllergens(Day, &UniqueAllergens);

    // @Incomplete: Hardcoded for 8 Allergens.
    Set_t<String_t> AllPossibleIngredients[8] = {};
    for (Int64_t i = 0; i < UniqueAllergens.NumEntries; ++i) {
        Set_t<String_t>* PossibleIngredients = &AllPossibleIngredients[i];
        FindPossibleIngredients(Day, UniqueAllergens.Entries[i], PossibleIngredients);
    }

    for (Int64_t i = 0; i < UniqueAllergens.NumEntries; ++i) {
        String_t Allergen = UniqueAllergens.Entries[i];
        Set_t<String_t>* PossibleIngredients = &AllPossibleIngredients[i];

        for (Int64_t j = 0; j < Day->NumFoods; ++j) {
            Food_t* Food = &Day->Foods[j];

            if (Contains(Food->Allergens, Food->NumAllergens, Allergen)) {
                RemoveIncompatibleIngredients(PossibleIngredients, Food->Ingredients, Food->NumIngredients);
            }
        }
    }

    while (AllergenIngredientPairs->NumEntries < UniqueAllergens.NumEntries) {
        for (Int64_t i = 0; i < UniqueAllergens.NumEntries; ++i) {
            String_t Allergen = UniqueAllergens.Entries[i];
            Set_t<String_t>* PossibleIngredients = &AllPossibleIngredients[i];
            if (PossibleIngredients->NumEntries == 1) {
                String_t Ingredient = PossibleIngredients->Entries[0];

                Add(DangerousIngredients, Ingredient);

                AllergenIngredient_t AllergenIngredient = {};
                AllergenIngredient.Allergen = Allergen;
                AllergenIngredient.Ingredient = Ingredient;
                Add(AllergenIngredientPairs, AllergenIngredient);
                for (Int64_t j = 0; j < UniqueAllergens.NumEntries; ++j) {
                    Remove(&AllPossibleIngredients[j], Ingredient);
                }
            }
        }
    }
}

Int64_t SolvePart1(Day21_t* Day, Set_t<String_t>* DangerousIngredients) {
    Int64_t Count = 0;
    for (Int64_t i = 0; i < Day->NumFoods; ++i) {
        Food_t* Food = &Day->Foods[i];

        for (Int64_t j = 0; j < Food->NumIngredients; ++j) {
            if (!Contains(DangerousIngredients, Food->Ingredients[j])) {
                Count += 1;
            }
        }
    }

    return Count;
}

void SolvePart2(Day21_t* Day, Array_t<AllergenIngredient_t>* AllergenIngredientPairs) {
    Array_t<AllergenIngredient_t> OrderedAllergenIngredients = {};
    for (Int64_t i = 0; i < AllergenIngredientPairs->NumEntries; ++i) {
        Int64_t Index = -1;
        for (Int64_t j = 0; j < OrderedAllergenIngredients.NumEntries; ++j) {
            if (StringCompare(AllergenIngredientPairs->Entries[i].Allergen, OrderedAllergenIngredients.Entries[j].Allergen) < 0) {
                Index = j;
                break;
            }
        }

        if (Index == -1) {
            Add(&OrderedAllergenIngredients, AllergenIngredientPairs->Entries[i]);
        } else {
            Insert(&OrderedAllergenIngredients, AllergenIngredientPairs->Entries[i], Index);
        }
    }

    for (Int64_t i = 0; i < OrderedAllergenIngredients.NumEntries; ++i) {
        printf("%s", OrderedAllergenIngredients.Entries[i].Ingredient.Name);

        if (i != (OrderedAllergenIngredients.NumEntries - 1)) {
            printf(",");
        }
    }
}

Int32_t main(Int32_t Argc, Char_t* Argv[]) {
    Input_t Input = {};
    if (!LoadInput(&Input, "Data\\Input.txt")) {
        return EXIT_FAILURE;
    }

    Day21_t Day = {};
    Int64_t Offset = 0;
    while (Offset < Input.Length) {
        Day.NumFoods += 1;
        Assert(Day.NumFoods <= MAX_FOOD_COUNT);
        Food_t* Food = &Day.Foods[Day.NumFoods - 1];

        while (Input.Data[Offset] != '(') {
            Food->NumIngredients += 1;
            Assert(Food->NumIngredients <= MAX_INGREDIENT_COUNT);
            String_t* Ingredient = &Food->Ingredients[Food->NumIngredients - 1];

            Offset += ScanForString(Input.Data + Offset, MAX_STRING_LENGTH, Ingredient->Name, &Ingredient->Length);
            Offset += 1; // Eat the space.
        }

        Offset += 10; // Eat the "(contains ".

        while (Input.Data[Offset] != '\n') {
            Food->NumAllergens += 1;
            Assert(Food->NumAllergens <= MAX_ALLERGEN_COUNT);
            String_t* Allergen = &Food->Allergens[Food->NumAllergens - 1];

            Offset += ScanForEnglish(Input.Data + Offset, MAX_STRING_LENGTH, Allergen->Name, &Allergen->Length);
            Offset += 1; // Eat the comma or the close parenthesis.

            if (Input.Data[Offset] == ' ') {
                Offset += 1; // Eat the space.
            }
        }

        Offset += 1; // Eat the newline.
    }

    Set_t<String_t> DangerousIngredients = {};
    Array_t<AllergenIngredient_t> AllergenIngredientPairs = {};
    IdentifyAllergensAndIngredients(&Day, &DangerousIngredients, &AllergenIngredientPairs);

    printf("Part 1: %lld\n", SolvePart1(&Day, &DangerousIngredients));

    printf("Part 2: ");
    SolvePart2(&Day, &AllergenIngredientPairs);

    return EXIT_SUCCESS;
}
