#include "math.h"
#include "stdio.h"

float fastfloor(float f)
{
    int sign;

    if (f<0.0f)
    {
        return (float)((int)(f-0.99f));
    }
    else
    {
        return (float)((int)(f));
    }
}
void main(int argc,char *argv[])
{
    float f;
    int i;

    f = -2.0f;
    for (i=0;i<30;i++)
    {
        printf("floor(%f) = %f,fastfloor = %f\n",f,floor(f),fastfloor(f));
        f += 0.13f;
    }
}