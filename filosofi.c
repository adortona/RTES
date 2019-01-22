#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_FILOSOFI 5
#define THINKING 10
#define EATING 20
#define DELAY 9999

sem_t mutex;
sem_t chopstick[NUM_FILOSOFI];

int state;

void init(){

    sem_init(&mutex,0,1);
    for(int i = 0; i < NUM_FILOSOFI; i++){
        sem_init(&chopstick[i],0,1);
    }
    
}

void *filosofo(int i){
    state = THINKING;
    for(int j = 0; j < DELAY; j++);
    sem_wait(&chopstick[i]);
    sem_wait(&chopstick[(i+1)% NUM_FILOSOFI] );
    state = EATING;
    printf("il filosofo numero %d sta mangiando", i);
    printf("\n");
    for(int k = 0; k < DELAY; k++);
    sem_post(&chopstick[i]);
    sem_post(&chopstick[(i+1)% NUM_FILOSOFI]);  
    state = THINKING;  
    return 0;
}

void *filosofo_mancino(int i){
    state = THINKING;
    for(int j = 0; j < DELAY; j++);
    sem_wait(&chopstick[(i+1)% NUM_FILOSOFI]);
    sem_wait(&chopstick[i]);
    state = EATING;
    printf("il filosofo numero %d sta mangiando", i);
    printf("\n");
    for(int k = 0; k < DELAY; k++);
    sem_post(&chopstick[(i+1)% NUM_FILOSOFI]);  
    sem_post(&chopstick[i]);
    state = THINKING;  
    return 0;
}


int main()
{
    pthread_attr_t attr;
    pthread_t t1, t2, t3, t4, t5;

    init();

    pthread_attr_init(&attr);

    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    

    pthread_create(&t1, &attr, filosofo, 0);
    pthread_create(&t2, &attr, filosofo, 1);
    pthread_create(&t3, &attr, filosofo, 2);
    pthread_create(&t4, &attr, filosofo, 3);
    pthread_create(&t5, &attr, filosofo_mancino, 4);
    

   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t3, NULL);
   pthread_join(t4, NULL);
   pthread_join(t5, NULL);



    return 0;
}

