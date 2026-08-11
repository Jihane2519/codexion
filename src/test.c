#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int x = 2;
int mails = 0;
int lock = 0;
pthread_mutex_t mutex;
void* routine1()
{
    // printf("test from threads\n");
    // sleep(3);
    // printf("Ending thread\n");
    printf("Process id %d\n", getpid());
    x += 5;
    sleep(2);
    printf("value of x: %d\n", x);
    return NULL;
}

void* routine2()
{
    // printf("test from threads\n");
    // sleep(3);
    // printf("Ending thread\n");
    printf("Process id %d\n", getpid());
    // x++;
    // sleep(2);
    printf("value of x: %d\n", x);
    return NULL;
}

void* routine()
{
    for (int i = 0; i < 1000000; i++) {
        // if (lock == 1){
        //     // wait until the lock is 0
        // }
        // lock = 1;
        pthread_mutex_lock(&mutex);
        // read mails
        // increment
        // write mails
        mails++;
        // lock = 0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* routine3(){
    int value = (rand() % 6) + 1;
    int* result = malloc(sizeof(int));
    *result = value;
    printf("thread result : %p\n", result);
    return (void *)result;
}

int main(){
    pthread_t th[4];
    int i = 0;
    int* res;
    pthread_mutex_init(&mutex, NULL);
    // si on fait create and join threads in the same loop 
    // threads ne sont pas paralele car pour chaqu'une 
    // on va creer et attender puis on pass a l'autre
    // solution boucle pour creer et une autre pour join 
    for (i = 0; i < 2; i++){
        if (pthread_create(th + i, NULL, &routine3, NULL) != 0){
            perror("Failed to create thread");
            return 1;
        }
        printf("Thread %d has started \n", i);
        if (pthread_join(th[i], (void**) &res) != 0){
            return 2;
        }
        free(res);
        printf("Thread %d has finished \n", i);
    }
    // if (pthread_create(&t1, NULL, &routine, NULL) != 0){
    //     return 1;
    // }
    // if (pthread_create(&t2, NULL, &routine, NULL) != 0){
    //     return 2;
    // }
    // if (pthread_join(t1, NULL) != 0){
    //     return 3;
    // }
    // if (pthread_join(t2, NULL) != 0){
    //     return 4;
    // }
    pthread_mutex_destroy(&mutex);
    printf("the main programme\n");
    printf("mails : %d", mails);
    return 0;
}