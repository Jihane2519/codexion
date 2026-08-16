// #ifndef HEADER_H
// # define HEADER_H

// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>

// typedef enum e_scheduler
// {
//     FIFO,
//     EDF
// } t_scheduler;


// typedef struct s_config
// {
//     int         nbr_of_coders;
//     int         time_to_burnout;
//     int         time_to_compile;
//     int         time_to_debug;
//     int         time_to_refactor;
//     int         nbr_of_compiles_required;
//     int         dongle_cooldown;
//     t_scheduler scheduler;
// } t_config;

// typedef struct s_dongle
// {
//     pthread_mutex_t pause_dongle;
//     t_edf   queue[2];
//     int size;
//     int is_taken;
//     long release; //pour cooldown

// } t_dongle;


// typedef struct s_edf
// {
//     int     id;
//     long    deadline;
// } t_edf;

// typedef struct s_simulation
// {
//     pthread_mutex_t pause_print;
//     t_config    config;
//     t_coder     *coders; // list of coder coders
//     //   │
//     //   ▼
//     // ┌────────┬────────┬────────┬────────┐
//     // │ Coder1 │ Coder2 │ Coder3 │ Coder4 │
//     // └────────┴────────┴────────┴────────┘
//     t_dongle    *dongles; 
//     //┌────┬────┬────┬────┐
//     //│ D1 │ D2 │ D3 │ D4 │
//     //└────┴────┴────┴────┘
//     int     simulation_running; // 1-> simulation continue 0-> doit s'arreter
//     long    start_time; //Le temps auquel la simulation commence (burnout, deadline, logs)
//     pthread_cond_t wake_up;
// } t_simulation;

// typedef struct s_coder
// {
//     int     id;
//     int     done; // pas terminer / terminer
//     pthread_t       thread;
//     t_dongle *left_dongle;
//     t_dongle *right_dongle;
//     long    last_compile;
//     int     nbr_of_compilations; // nombre de compilations déjà effectuées
//     t_simulation    *sim;
//     char *color;
// } t_coder;

// #endif



// la version corriger 
#ifndef HEADER_H
# define HEADER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

typedef struct s_simulation t_simulation;
typedef enum e_scheduler
{
    FIFO,
    EDF
} t_scheduler;

typedef struct s_edf
{
    int     id;
    long    deadline;
} t_edf;


typedef struct s_config
{
    int         nbr_of_coders;
    int         time_to_burnout;
    int         time_to_compile;
    int         time_to_debug;
    int         time_to_refactor;
    int         nbr_of_compiles_required;
    int         dongle_cooldown;
    t_scheduler scheduler;
} t_config;

typedef struct s_dongle
{
    pthread_mutex_t pause_dongle;
    t_edf           quee[2];
    int             size;
    int             is_taken;
    long            release;
} t_dongle;

typedef struct s_coder
{
    int             id;
    int             done;
    pthread_t       thread;
    t_dongle        *left_dongle;
    t_dongle        *right_dongle;
    long            last_compile;
    int             nbr_of_compilations;
    t_simulation    *sim;
    char            *color;
} t_coder;

typedef struct s_simulation
{
    pthread_mutex_t pause_print;
    pthread_mutex_t pause;
    t_config        config;
    t_coder         *coders;
    t_dongle        *dongles;
    int             simulation_running;
    long            start_time;
    pthread_cond_t  wake_up;
} t_simulation;

int     parse_args(int argc, char **argv, t_config *config);
int     init_simulation(t_simulation *sim, t_config *config);
void    cleanup_simulation(t_simulation *sim);
void    *coder_routine(void *arg);
int    take_dongle(t_coder *coder, t_dongle *dongle);
void    release_dongle(t_dongle *dongle);
long    get_time_ms(void);
void    insert_heap(t_dongle *dongle, t_edf info);
void    insert_down(t_dongle *dongle);
t_edf   pop_heap(t_dongle *dongle);
void    *coder_routine(void *arg);
int     compile(t_coder *coder);
void    debug(t_coder *coder);
void    refactor(t_coder *coder);
void    *monitor_routine(void *arg);

#endif

//pthread_mutex_t pause
//leur role est de proteger les donnes que plusieurs
//  threads peuvent modifier/lire simultanement par exemple 
// sim->simulation_running
//  coder->done 
// coder->last_compile 
// coder->nbr_of_compilations