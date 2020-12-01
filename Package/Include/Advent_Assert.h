#ifndef Advent_Assert_h
#define Advent_Assert_h 1

#include <cstdlib>

#define Assert_(Condition, File, Line) do {     \
    if ((Condition) == false) {                 \
        printf("[ASSERT] %s.\n", #Condition);   \
        printf("    at %s:%d.\n", File, Line);  \
        __debugbreak();                         \
    }                                           \
} while (false)

#define Assert(Condition) \
    Assert_(Condition, __FILE__, __LINE__)

#endif // Advent_Assert_h
