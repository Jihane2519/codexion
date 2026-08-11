#include "header.h"

int main(int argc, char **argv)
{
    t_config    config;
    t_simulation    sim;
    int             i;
    // parser
    if (parse_args(argc, argv, &config))
        return (1);

    // init simulation
    if (init_simulation(&sim, &config))
        return (1);
    // lancer les coders
    i = 0;
    while (i < sim.config.nbr_of_coders)
    {
        pthread_create(
            &sim.coders[i].thread,
            NULL,
            coder_routine,
            &sim.coders[i]
        );
        i++;
    }
    // join
    i = 0;
    while(i < sim.config.nbr_of_coders)
    {
        pthread_join(sim.coders[i].thread, NULL); //attention si ona t_simulation *sim tu dois remplacer 
        // par sim->coders[i].thread
        i++;
    }
    // 5. cleanup
    cleanup_simulation(&sim);
    printf("Codexion started!\n");
    return 0;
}