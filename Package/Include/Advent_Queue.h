#ifndef Advent_Queue_h
#define Advent_Queue_h 1

#include "Advent_Assert.h"
#include "Advent_Types.h"

#define MAX_QUEUE_ENTRY_COUNT 1024

template <typename Data_t>
struct Queue_t {
    Int64_t NumEntries;
    Data_t Entries[MAX_QUEUE_ENTRY_COUNT];
};

template <typename Data_t>
Bool_t Add(Queue_t<Data_t>* Queue, Data_t Value) {
    Queue->NumEntries += 1;
    Assert(Queue->NumEntries <= MAX_QUEUE_ENTRY_COUNT);
    Queue->Entries[Queue->NumEntries - 1] = Value;
    return true;
}

template <typename Data_t>
Bool_t Remove(Queue_t<Data_t>* Queue) {
    Queue->NumEntries -= 1;
    Assert(Queue->NumEntries >= 0);

    for (Int64_t i = 0; i < Queue->NumEntries; ++i) {
        Queue->Entries[i] = Queue->Entries[i + 1];
    }

    return true;
}

template <typename Data_t>
Bool_t Peek(Queue_t<Data_t>* Queue, Data_t* Value) {
    if (Queue->NumEntries <= 0) {
        return false;
    }

    *Value = Queue->Entries[0];
    return true;
}

template <typename Data_t>
Bool_t Pop(Queue_t<Data_t>* Queue, Data_t* Value) {
    if (Peek(Queue, Value)) {
        return Remove(Queue);
    }

    return false;
}

#endif // Advent_Queue_h
