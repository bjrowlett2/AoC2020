#ifndef Advent_Array_h
#define Advent_Array_h

#include "Advent_Assert.h"
#include "Advent_Types.h"

#define MAX_ARRAY_ENTRY_COUNT 128

template <typename Data_t>
struct Array_t {
    Int64_t NumEntries;
    Data_t Entries[MAX_ARRAY_ENTRY_COUNT];
};

template <typename Data_t>
Bool_t Add(Array_t<Data_t>* Array, Data_t Value) {
    Array->NumEntries += 1;
    Assert(Array->NumEntries <= MAX_ARRAY_ENTRY_COUNT);
    Array->Entries[Array->NumEntries - 1] = Value;
    return true;
}

template <typename Data_t>
Bool_t Insert(Array_t<Data_t>* Array, Data_t Value, Int64_t Index) {
    Assert(Index <= Array->NumEntries);
    Assert(Array->NumEntries != MAX_ARRAY_ENTRY_COUNT);

    // Shift everything [Index, NumEntries) up by one.
    for (Int64_t i = Array->NumEntries - 1; i >= Index; --i) {
        Array->Entries[i + 1] = Array->Entries[i];
    }

    Array->NumEntries += 1;
    Assert(Array->NumEntries <= MAX_ARRAY_ENTRY_COUNT);
    Array->Entries[Index] = Value;
    return true;
}

#endif // Advent_Array_h
