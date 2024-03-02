#include "utils.h"

inline float clamp(float val, float min, float max)
{
    if(val > max)
        return max;
    if(val < min)
        return min;
    return val;
}

inline int random(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}
