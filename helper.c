#include "helper.h"

int min(int a, int b)
{
    return a <= b ? a : b;
}

int max(int a, int b)
{
    return a >= b ? a : b;
}

int random_range(int a, int b)
{
    return a + rand() % (b - a + 1);
}

int square(int x)
{
    return x * x;
}