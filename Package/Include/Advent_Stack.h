#ifndef Advent_Stack_h
#define Advent_Stack_h 1

#define MAX_STACK_ENTRY_COUNT 1024

template <typename Data_t>
struct Stack_t {
    Int64_t NumEntries;
    Data_t Entries[MAX_STACK_ENTRY_COUNT];
};

template <typename Data_t>
Bool_t Add(Stack_t<Data_t>* Stack, Data_t Value) {
    Stack->NumEntries += 1;
    Assert(Stack->NumEntries <= MAX_STACK_ENTRY_COUNT);
    Stack->Entries[Stack->NumEntries - 1] = Value;
    return true;
}

template <typename Data_t>
Bool_t Remove(Stack_t<Data_t>* Stack) {
    Stack->NumEntries -= 1;
    Assert(Stack->NumEntries >= 0);
    return true;
}

template <typename Data_t>
Bool_t Peek(Stack_t<Data_t>* Stack, Data_t* Value) {
    if (Stack->NumEntries <= 0) {
        return false;
    }

    Int64_t Index = Stack->NumEntries - 1;
    *Value = Stack->Entries[Index];
    return true;
}

template <typename Data_t>
Bool_t Pop(Stack_t<Data_t>* Stack, Data_t* Value) {
    if (Peek(Stack, Value)) {
        return Remove(Stack);
    }

    return false;
}

#endif // Advent_Stack_h
