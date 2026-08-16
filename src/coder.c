#include "header.h"

// coder_routine()
//     ↓
// take_dongle()
//     ↓
// compile()
//     ↓
// release_dongle()
//     ↓
// debug()
//     ↓
// refactor()

void *coder_routine(void *arg)
{
    t_coder *coder;
    coder =(t_coder *)arg;
    printf("Coder %d started\n", coder->id);
    while(coder->done == 0)
    {
        pthread_mutex_lock(&coder->sim->pause);
        if (coder->sim->simulation_running == 0)
        {
            pthread_mutex_unlock(&coder->sim->pause);
            break;
        }
        pthread_mutex_unlock(&coder->sim->pause);
        if (compile(coder))
            break;
        //if (coder->done 
        // && coder->last_compile 
        // + coder->sim->config.time_to_debug 
        // + coder->sim->config.time_to_refactor 
        // > coder->sim->config->time_to_burnout)
        //  break;
        debug(coder);
        refactor(coder);
    }
    return (NULL);
    
}


static void coder_sleep(t_coder *coder, long duration)
{
    long start;

    start = get_time_ms();
    while (get_time_ms() - start < duration)
    {
        pthread_mutex_lock(&coder->sim->pause);
        if (coder->sim->simulation_running == 0)
        {
            pthread_mutex_unlock(&coder->sim->pause);
            return;
        }
        pthread_mutex_unlock(&coder->sim->pause);
        usleep(1000);
    }
}

int     compile(t_coder *coder)
{
    long    compile_start;
    if (coder->id % 2 == 0)
    {
        if (take_dongle(coder, coder->left_dongle))
            return (1);
        if (take_dongle(coder, coder->right_dongle))
        {
            release_dongle(coder->left_dongle);
            return (1);
        }
    }
    else
    {
        if (take_dongle(coder, coder->right_dongle))
            return (1);
        if (take_dongle(coder, coder->left_dongle))
        {
            release_dongle(coder->right_dongle);
            return (1);
        }
    }
    compile_start = get_time_ms();
    pthread_mutex_lock(&coder->sim->pause_print);
    printf("%ld %d is compiling\n",
        compile_start - coder->sim->start_time,
        coder->id);
    pthread_mutex_unlock(&coder->sim->pause_print);
    pthread_mutex_lock(&coder->sim->pause);
    coder->last_compile = compile_start;
    coder->nbr_of_compilations++;
    pthread_mutex_unlock(&coder->sim->pause);
    coder_sleep(coder, coder->sim->config.time_to_compile);
    release_dongle(coder->left_dongle);
    release_dongle(coder->right_dongle);
    return (0);
}

void    debug(t_coder *coder)
{
    long debug_start;

    debug_start = get_time_ms();
    pthread_mutex_lock(&coder->sim->pause_print);
    printf("%ld %d is debugging\n",
        debug_start - coder->sim->start_time,
        coder->id);
    pthread_mutex_unlock(&coder->sim->pause_print);
    coder_sleep(coder, coder->sim->config.time_to_debug);
}


void    refactor(t_coder *coder)
{
    long refactor_start;

    refactor_start = get_time_ms();
    pthread_mutex_lock(&coder->sim->pause_print);
    printf("%ld %d is refactoring\n",
        refactor_start - coder->sim->start_time, 
        coder->id);
    pthread_mutex_unlock(&coder->sim->pause_print);
    coder_sleep(coder, coder->sim->config.time_to_refactor);
    pthread_mutex_lock(&coder->sim->pause);
    //dans refactor, Sinon tu fais un cycle complet après la dernière compilation
    if (coder->nbr_of_compilations 
        == coder->sim->config.nbr_of_compiles_required)
        coder->done = 1;
    pthread_mutex_unlock(&coder->sim->pause);

}
// void compile(t_coder *coder)
// {
//     long compile_start;

//     take_dongle(coder, coder->left_dongle);
//     take_dongle(coder, coder->right_dongle);

//     compile_start = get_time_ms();

//     pthread_mutex_lock(&coder->sim->pause_print);
//     printf("%ld %d is compiling\n",
//         compile_start - coder->sim->start_time,
//         coder->id);
//     pthread_mutex_unlock(&coder->sim->pause_print);

//     pthread_mutex_lock(&coder->sim->pause);
//     coder->last_compile = compile_start;
//     coder->number_of_compilations++;
//     pthread_mutex_unlock(&coder->sim->pause);

//     coder_sleep(coder, coder->sim->config.time_to_compile);

//     release_dongle(coder->left_dongle);
//     release_dongle(coder->right_dongle);
// }