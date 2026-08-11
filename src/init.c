#include "header.h"

int init_simulation(t_simulation *sim, t_config *config)
{
    int i;

    sim->config = *config;
    sim->simulation_running = 1;
    sim->start_time = 0;

    pthread_mutex_init(&sim->pause_print, NULL);
    pthread_mutex_init(&sim->pause, NULL);
    pthread_cond_init(&sim->wake_up, NULL);

    sim->coders = malloc(sizeof(t_coder)
        * sim->config.nbr_of_coders);
    if (!sim->coders)
        return (1);

    sim->dongles = malloc(sizeof(t_dongle)
        * sim->config.nbr_of_coders);
    if (!sim->dongles)
    {
        free(sim->coders);
        return (1);
    }

    i = 0;
    while (i < sim->config.nbr_of_coders)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].done = 0;
        sim->coders[i].left_dongle = &sim->dongles[i];
        sim->coders[i].right_dongle =
            &sim->dongles[(i + 1) % sim->config.nbr_of_coders];
        sim->coders[i].nbr_of_compilations = 0;
        sim->coders[i].sim = sim;
        sim->coders[i].last_compile = 0;

        sim->dongles[i].size = 0;
        sim->dongles[i].is_taken = 0;
        sim->dongles[i].release = 0;

        pthread_mutex_init(
            &sim->dongles[i].pause_dongle,
            NULL
        );

        i++;
    }

    return (0);
}
void        cleanup_simulation(t_simulation *sim)
{
    int i;
    i = 0;
    while (i < sim->config.nbr_of_coders)
    {
        pthread_mutex_destroy(&sim->dongles[i].pause_dongle);
        i++;
    }
    pthread_mutex_destroy(&sim->pause_print);
    pthread_mutex_destroy(&sim->pause);
    pthread_cond_destroy(&sim->wake_up);

    //free les deux malloc de init_simulation
    free(sim->coders);
    free(sim->dongles);

}