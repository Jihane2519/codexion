// #include "header.h"

// // int main(int argc, char **argv)
// // {
// //     t_config    config;
// //     t_simulation    sim;
// //     int             i;
// //     // parser
// //     if (parse_args(argc, argv, &config))
// //         return (1);

// //     // init simulation
// //     if (init_simulation(&sim, &config))
// //         return (1);
// //     // lancer les coders
// //     i = 0;
// //     while (i < sim.config.nbr_of_coders)
// //     {
// //         pthread_create(
// //             &sim.coders[i].thread,
// //             NULL,
// //             coder_routine,
// //             &sim.coders[i]
// //         );
// //         i++;
// //     }
// //     // join
// //     i = 0;
// //     while(i < sim.config.nbr_of_coders)
// //     {
// //         pthread_join(sim.coders[i].thread, NULL); //attention si ona t_simulation *sim tu dois remplacer 
// //         // par sim->coders[i].thread
// //         i++;
// //     }
// //     // 5. cleanup
// //     cleanup_simulation(&sim);
// //     printf("Codexion started!\n");
// //     return 0;
// // }


// // int main(void)
// // {
// //     t_simulation sim;
// //     t_config config;
// //     t_coder coder1;
// //     t_coder coder2;

// //     /* remplir config avec des valeurs simples */

// //     init_simulation(&sim, &config);

// //     coder1.id = 1;
// //     coder2.id = 2;
// //     coder1.sim = &sim;
// //     coder1.last_compile = sim.start_time;
// //     coder2.sim = &sim;
// //     coder2.last_compile = sim.start_time;

// //     take_dongle(&coder1, &sim.dongles[0]);
// //     take_dongle(&coder2, &sim.dongles[0]);

// //     printf("Dongle taken: %d\n",
// //         sim.dongles[0].is_taken);

// //     release_dongle(&sim.dongles[0]);

// //     printf("Dongle taken: %d\n",
// //         sim.dongles[0].is_taken);

// //     return (0);
// // }

// void *test_coder(void *arg)
// {
//     t_coder *coder;

//     coder = (t_coder *)arg;

//     take_dongle(coder, &coder->sim->dongles[0]);

//     printf("Coder %d is using the dongle\n", coder->id);

//     usleep(2000000);

//     release_dongle(&coder->sim->dongles[0]);

//     printf("Coder %d released the dongle\n", coder->id);

//     return (NULL);
// }

// int main(void)
// {
//     t_simulation sim;
//     t_config config;
//     t_coder coder1;
//     t_coder coder2;
//     pthread_t thread1;
//     pthread_t thread2;

//     /* remplir config */

//     init_simulation(&sim, &config);

//     coder1.id = 1;
//     coder1.sim = &sim;
//     coder1.last_compile = sim.start_time;

//     coder2.id = 2;
//     coder2.sim = &sim;
//     coder2.last_compile = sim.start_time;

//     pthread_create(&thread1, NULL, test_coder, &coder1);
//     pthread_create(&thread2, NULL, test_coder, &coder2);

//     pthread_join(thread1, NULL);
//     pthread_join(thread2, NULL);

//     return (0);
// }
#include "header.h"

void *fake_monitor(void *arg)
{
    t_simulation *sim;

    sim = (t_simulation *)arg;
    usleep(5000000);
    sim->simulation_running = 0;
    return (NULL);
}

int main(void)
{
    t_simulation    sim;
    t_config        config;
    pthread_t       monitor;
    int             i;

    config.nbr_of_coders = 3;
    config.time_to_burnout = 500;
    config.time_to_compile = 100;
    config.time_to_debug = 100;
    config.time_to_refactor = 100;
    config.nbr_of_compiles_required = 2;
    config.dongle_cooldown = 0;
    config.scheduler = EDF;

    if (init_simulation(&sim, &config))
        return (1);
    //test edf
    sim.coders[0].last_compile = get_time_ms() - 100;
    sim.coders[1].last_compile = get_time_ms() - 400;
    sim.coders[2].last_compile = get_time_ms() - 200;

    pthread_create(&monitor, NULL, fake_monitor, &sim);

    i = 0;
    while (i < sim.config.nbr_of_coders)
    {
        pthread_create(&sim.coders[i].thread, NULL,
            coder_routine, &sim.coders[i]);
        i++;
    }

    i = 0;
    while (i < sim.config.nbr_of_coders)
    {
        pthread_join(sim.coders[i].thread, NULL);
        i++;
    }

    pthread_join(monitor, NULL);
    cleanup_simulation(&sim);
    return (0);
}