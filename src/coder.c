#include "header.h"


void *coder_routine(void *arg)
{
    t_coder *coder;
    coder =(t_coder *)arg;
    printf("Coder %d started\n", coder->id);
    return (NULL);
}