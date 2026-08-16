#include "header.h"

static long get_deadline(t_coder *coder)
{
    return (coder->last_compile
        + coder->sim->config.time_to_burnout);
}

static void set_deadline(t_coder *coder, t_edf *info)
{
    info->id = coder->id;
    if (coder->sim->config.scheduler == FIFO)
        info->deadline = get_time_ms();
    else
        info->deadline = get_deadline(coder);
 }

// dongle->release = get_time_ms() le moment où le dongle a été libéré
// 1. condition OK → return(0)  ← prendre le dongle
//         ↓
// 2. condition pas OK → unlock + usleep(1000)
//         ↓
// 3. simulation_running == 0 → return(1)  ← sortir proprement
//         ↓
// 4. sinon → re-lock + revérifier
static int wait_dongle(t_coder *coder, t_dongle *dongle)
 {
    while(1)
    {
        if (dongle->size > 0 
            && dongle->is_taken == 0 
            && dongle->quee[0].id == coder->id 
            && get_time_ms() - dongle->release 
                >= coder->sim->config.dongle_cooldown)
                return(0);
        //Je ne peux pas prendre le dongle maintenant, donc 
        // je libère le mutex pour que les autres threads puissent travailler.
        pthread_mutex_unlock(&dongle->pause_dongle);
        usleep(1000); //1ms
        // on va faire verification de simulation stop
        pthread_mutex_lock(&coder->sim->pause);
        if (coder->sim->simulation_running == 0)
        {
            pthread_mutex_unlock(&coder->sim->pause);
            return (1);
        }
        pthread_mutex_unlock(&coder->sim->pause);
         //Je reprends le mutex pour refaire mes vérifications en toute sécurité
        //reprendre le contrôle protégé du dongle pour pouvoir 
        // vérifier à nouveau
        pthread_mutex_lock(&dongle->pause_dongle);
    }

 }


//  LOCK
//   ↓
// ajouter ma demande dans la heap
//   ↓
// attendre mon tour
//   ↓
// retirer ma demande de la heap
//   ↓
// marquer le dongle comme pris
//   ↓
// afficher
//   ↓
// UNLOCK
 int  take_dongle(t_coder *coder, t_dongle *dongle)
 {
    t_edf info;
    t_edf taken;
    pthread_mutex_lock(&dongle->pause_dongle);
    set_deadline(coder, &info);
    insert_heap(dongle, info);
    if(wait_dongle(coder, dongle))
    {
        pthread_mutex_unlock(&dongle->pause_dongle);
        return 1;
    }
    taken = pop_heap(dongle);
    dongle->is_taken = 1;
    printf("%ld %d has taken a dongle\n",
        get_time_ms() - coder->sim->start_time,
        taken.id);
    pthread_mutex_unlock(&dongle->pause_dongle);
    return 0;
 }

void    release_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->pause_dongle);
    // dongle->release = get_time_ms();
    // dongle->is_taken = 0;// cette ordre va causer un probleme en cas de coldown 0
    dongle->is_taken = 0;
    dongle->release = get_time_ms();
    pthread_mutex_unlock(&dongle->pause_dongle);
}

// void release_dongle(t_dongle *dongle)
// {
//     pthread_mutex_lock(&dongle->pause_dongle);

//     dongle->release = get_time_ms();
//     dongle->is_taken = 0;

//     pthread_cond_broadcast(&dongle->dongle_cond);

//     pthread_mutex_unlock(&dongle->pause_dongle);
// }

// static int wait_dongle(t_coder *coder, t_dongle *dongle)
// {
//     while (1)
//     {
//         if (coder->sim->simulation_running == 0)
//             return (1);

//         if (dongle->is_taken == 0
//             && dongle->size > 0
//             && dongle->quee[0].id == coder->id
//             && get_time_ms() - dongle->release
//                 >= coder->sim->config.dongle_cooldown)
//         {
//             return (0);
//         }

//         pthread_cond_wait(&dongle->dongle_cond,
//                           &dongle->pause_dongle);
//     }
// }

 // take_dongle()
// coder veut un dongle
//         ↓
// créer son t_edf
//         ↓
// insert_heap()
//         ↓
// le coder attend
//         ↓
// est-ce qu'il est en tête de queue ?
//         ↓
//     NON → attendre
//         ↓
//     OUI
//         ↓
// le dongle est-il disponible ?
//         ↓
//     NON → attendre
//         ↓
//     OUI
//         ↓
// pop_heap()
//         ↓
// is_taken = 1
//         ↓
// "has taken a dongle"
