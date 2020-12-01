#ifndef Advent_Panic_h
#define Advent_Panic_h 1

#include <cstdlib>

#define Panic_(Message, File, Line) do {    \
    printf("[PANIC] %s.\n", Message);       \
    printf("    at %s:%d.\n", File, Line);  \
    abort();                                \
} while (false)

#define Panic(Message) \
    Panic_(Message, __FILE__, __LINE__)

#endif // Advent_Panic_h
