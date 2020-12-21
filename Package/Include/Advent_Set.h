#ifndef Advent_Set_h
#define Advent_Set_h

#include "Advent_Assert.h"
#include "Advent_Types.h"

#define MAX_SET_ENTRY_COUNT 128

template <typename Data_t>
struct Set_t {
    Int64_t NumEntries;
    Data_t Entries[MAX_SET_ENTRY_COUNT];
};

template <typename Data_t>
Bool_t Contains(Set_t<Data_t>* Set, Data_t Value) {
    for (Int64_t i = 0; i < Set->NumEntries; ++i) {
        if (Equals(Set->Entries[i], Value)) {
            return true;
        }
    }

    return false;
}

template <typename Data_t>
Bool_t Add(Set_t<Data_t>* Set, Data_t Value) {
    if (!Contains(Set, Value)) {
        Set->NumEntries += 1;
        Assert(Set->NumEntries <= MAX_SET_ENTRY_COUNT);
        Set->Entries[Set->NumEntries - 1] = Value;
    }
    
    return true;
}

template <typename Data_t>
Bool_t Remove(Set_t<Data_t>* Set, Int64_t Index) {
    Assert(Index < Set->NumEntries);

    // Shift everything [Index, NumEntries) down by one.
    for (Int64_t i = Index; i < Set->NumEntries; ++i) {
        Set->Entries[i] = Set->Entries[i + 1];
    }

    Set->NumEntries -= 1;
    Assert(Set->NumEntries >= 0);
    return true;
}

template <typename Data_t>
Bool_t Remove(Set_t<Data_t>* Set, Data_t Value) {
    for (Int64_t i = 0; i < Set->NumEntries; ++i) {
        if (Equals(Set->Entries[i], Value)) {
            return Remove(Set, i);
        }
    }

    return false;
}

#endif // Advent_Array_h
