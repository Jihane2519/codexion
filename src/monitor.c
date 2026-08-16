// #include "header.h"

// void *monitor_routine(void *arg)
// {
//     t_simulation *sim;
//     int           i;
//     int           finish;

//     sim = (t_simulation *)arg;

//     while (1)
//     {
//         finish = 0;
//         i = 0;

//         while (i < sim->config.nbr_of_coders)
//         {
//             pthread_mutex_lock(&sim->pause);

//             if (sim->simulation_running == 0)
//             {
//                 pthread_mutex_unlock(&sim->pause);
//                 return (NULL);
//             }

//             if (sim->coder[i].done == 0 && get_time_ms() - sim->coders[i].last_compile
//                 > sim->config.time_to_burnout)
//             {
//                 sim->simulation_running = 0;

//                 pthread_mutex_unlock(&sim->pause);

//                 pthread_mutex_lock(&sim->pause_print);
//                 printf("%ld %d died\n",
//                     get_time_ms() - sim->start_time,
//                     sim->coders[i].id);
//                 pthread_mutex_unlock(&sim->pause_print);

//                 return (NULL);
//             }

//             if (sim->coders[i].done == 1)
//                 finish++;

//             pthread_mutex_unlock(&sim->pause);

//             i++;
//         }

//         if (finish == sim->config.nbr_of_coders)
//         {
//             pthread_mutex_lock(&sim->pause);
//             sim->simulation_running = 0;
//             pthread_mutex_unlock(&sim->pause);

//             return (NULL);
//         }

//         usleep(1000);
//     }

//     return (NULL);
// }


#include "header.h"

static int  check_burnout(t_simulation *sim, int i)
{
    int     burned_out;
    int     coder_id;
    long    last;
    long    start;

    burned_out = 0;
    pthread_mutex_lock(&sim->pause_print);
    pthread_mutex_lock(&sim->pause);
    if (sim->coders[i].last_compile == 0)
        last = sim->start_time;
    else
        last = sim->coders[i].last_compile;
    if (sim->coders[i].done == 0
        && get_time_ms() - last > sim->config.time_to_burnout)
    {
        sim->simulation_running = 0;
        burned_out = 1;
    }
    coder_id = sim->coders[i].id;
    start = sim->start_time;
    pthread_mutex_unlock(&sim->pause);
    if (burned_out)
        printf("%ld %d burned out\n",
            get_time_ms() - start, coder_id);
    pthread_mutex_unlock(&sim->pause_print);
    return (burned_out);
}

static int  check_coders(t_simulation *sim)
{
    int i;
    int done_count;

    i = 0;
    done_count = 0;
    while (i < sim->config.nbr_of_coders)
    {
        if (check_burnout(sim, i))
            return (-1);
        pthread_mutex_lock(&sim->pause);
        if (sim->coders[i].done == 1)
            done_count++;
        pthread_mutex_unlock(&sim->pause);
        i++;
    }
    return (done_count);
}

void    *monitor_routine(void *arg)
{
    t_simulation    *sim;
    int             done_count;

    sim = (t_simulation *)arg;
    pthread_mutex_lock(&sim->pause);
    while (sim->simulation_running == 1)
    {
        pthread_mutex_unlock(&sim->pause);
        usleep(1000);
        done_count = check_coders(sim);
        if (done_count == -1)
            return (NULL);
        if (done_count == sim->config.nbr_of_coders)
        {
            pthread_mutex_lock(&sim->pause);
            sim->simulation_running = 0;
            pthread_mutex_unlock(&sim->pause);
            return (NULL);
        }
        pthread_mutex_lock(&sim->pause);
    }
    pthread_mutex_unlock(&sim->pause);
    return (NULL);
}