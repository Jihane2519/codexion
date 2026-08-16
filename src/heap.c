#include "header.h"

void insert_heap(t_dongle *dongle, t_edf info)
{
    t_edf temp;
    int i;
    int parent;


    dongle->quee[dongle->size] = info;
    dongle->size++;

    i = dongle->size - 1;
    while (i > 0)
    {
        // i=1 parent (1-1) / 2 = 0 
        // donc le parent de 1 est 0
        parent = (i - 1) / 2;
        // nouvelle element n'est pas prioritaire
        if (dongle->quee[i].deadline
            > dongle->quee[parent].deadline)
            break;

        if (dongle->quee[i].deadline == dongle->quee[parent].deadline 
            && dongle->quee[i].id > dongle->quee[parent].id)
            break;

        temp = dongle->quee[i];
        dongle->quee[i] = dongle->quee[parent];
        dongle->quee[parent] = temp;
        i = parent;
    }
}


// void    insert_down(t_dongle *dongle)
// {
//     int index;
//     int left;
//     int right;
//     int smallest;

//     index = 0;
//     while(1)
//     {
//         left = 2 * index + 1;
//         right = 2 * index + 2;
//         //parent n'a plus d'enfant gauche
//         if (left >= dongle->size)
//             break;
//         smallest = left;
//         // rigth < dongle->size right is existe
//         if(right < dongle->size
//             && (dongle->quee[right].deadline
//                 < dongle->quee[left].deadline)
//             || (dongle->quee[right].deadline 
//                 == dongle->quee[left].deadline()
//             && (dongle->quee[right].id
//                 < dongle->quee[left].id)))
//             smallest = right;
        
//         if (dongle->quee[index].deadline
//             < dongle->quee[smallest].deadline || ((dongle->quee[index].deadline
//             == dongle->quee[smallest].deadline) && (dongle->quee[index].id
//             < dongle->quee[smallest].id)))
//             break;
//         temp = dongle->quee[index];
//         dongle->quee[index] = dongle->quee[smallest];
//         dongle->quee[smallest] = temp;
//         index = smallest;
//     }
// }

void insert_down(t_dongle *dongle)
{
    int     index;
    int     left;
    int     right;
    int     smallest;
    t_edf   temp;

    index = 0;
    while (1)
    {
        left = 2 * index + 1;
        right = 2 * index + 2;

        /* Le parent n'a plus d'enfant gauche */
        if (left >= dongle->size)
            break;

        /* On suppose que left est le meilleur enfant */
        smallest = left;

        /* // rigth < dongle->size right existe et si plus prioritaire que left */
        if (right < dongle->size
            && (dongle->quee[right].deadline
                < dongle->quee[left].deadline
                || (dongle->quee[right].deadline
                    == dongle->quee[left].deadline
                    && dongle->quee[right].id
                    < dongle->quee[left].id)))
        {
            smallest = right;
        }

        /* Le parent est déjà prioritaire */
        if (dongle->quee[index].deadline
            < dongle->quee[smallest].deadline
            || (dongle->quee[index].deadline
                == dongle->quee[smallest].deadline
                && dongle->quee[index].id
                < dongle->quee[smallest].id))
        {
            break;
        }

        /* Échange parent <-> meilleur enfant */
        temp = dongle->quee[index];
        dongle->quee[index] = dongle->quee[smallest];
        dongle->quee[smallest] = temp;

        /* On continue depuis la nouvelle position */
        index = smallest;
    }
}


t_edf   pop_heap(t_dongle *dongle)
{
    t_edf result;


    result = dongle->quee[0];
    dongle->quee[0] = dongle->quee[dongle->size - 1];
    dongle->size--;
    insert_down(dongle);
    return (result);
}