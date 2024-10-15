#include "x_types.hpp"

u8 test1(f32 value)
{
    f32 v;

    v = value;

    return (s8)value;
}

u8 test2(f32 value)
{
    union
    {
        f32 f;
        s32 i;
    } v;
    
    v.f = value + (1<<23);
    return v.i;
}
