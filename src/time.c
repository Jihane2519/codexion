#include "header.h"


long    get_time_ms(void)
{
    struct timeval  tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}


//elapsed = ecoulé, ms
long    elapsed_ms(long start)
{
    return (get_time_ms() - start);
}