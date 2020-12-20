#ifndef Advent_HashTable_h
#define Advent_HashTable_h 1

#include "Advent_Types.h"

template <typename Key_t, typename Data_t>
struct HashEntry_t {
    Key_t Key;
    Data_t Value;
};

#define MAX_HASH_ENTRY_COUNT 2048

template <typename Key_t, typename Data_t>
struct HashBucket_t {
    Int32_t NumEntries;
    HashEntry_t<Key_t, Data_t> Entries[MAX_HASH_ENTRY_COUNT];
};

#define MAX_HASH_BUCKET_COUNT 1024

template <typename Key_t, typename Data_t>
struct HashTable_t {
    HashBucket_t<Key_t, Data_t> Buckets[MAX_HASH_BUCKET_COUNT];
};

template <typename Key_t>
Uint32_t Hash(Key_t Key);

template <>
Uint32_t Hash(Int64_t Key) {
    return Key % MAX_HASH_BUCKET_COUNT;
}

template <typename Key_t>
Bool_t Equals(Key_t A, Key_t B) {
    return A == B;
}

template <typename Key_t, typename Data_t>
Bool_t Add(HashBucket_t<Key_t, Data_t>* HashBucket, Key_t Key, Data_t Value) {
    for (Int32_t i = 0; i < HashBucket->NumEntries; ++i) {
        HashEntry_t<Key_t, Data_t>* HashEntry = &HashBucket->Entries[i];

        if (Equals(HashEntry->Key, Key)) {
            HashEntry->Value = Value;
            return true;
        }
    }

    HashBucket->NumEntries += 1;
    Assert(HashBucket->NumEntries <= MAX_HASH_ENTRY_COUNT);
    HashEntry_t<Key_t, Data_t>* HashEntry = &HashBucket->Entries[HashBucket->NumEntries - 1];

    HashEntry->Key = Key;
    HashEntry->Value = Value;
    return true;
}

template <typename Key_t, typename Data_t>
Bool_t Add(HashTable_t<Key_t, Data_t>* HashTable, Key_t Key, Data_t Value) {
    Uint32_t Index = Hash(Key);
    return Add(&HashTable->Buckets[Index], Key, Value);
}

template <typename Key_t, typename Data_t>
Bool_t Find(HashBucket_t<Key_t, Data_t>* HashBucket, Key_t Key, Data_t* Value) {
    for (Int32_t i = 0; i < HashBucket->NumEntries; ++i) {
        HashEntry_t<Key_t, Data_t>* HashEntry = &HashBucket->Entries[i];

        if (Equals(HashEntry->Key, Key)) {
            *Value = HashEntry->Value;
            return true;
        }
    }

    return false;
}

template <typename Key_t, typename Data_t>
Bool_t Find(HashTable_t<Key_t, Data_t>* HashTable, Key_t Key, Data_t* Value) {
    Uint32_t Index = Hash(Key);
    return Find(&HashTable->Buckets[Index], Key, Value);
}

#endif // Advent_HashTable_h
