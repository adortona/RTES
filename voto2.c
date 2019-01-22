#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define SOGLIA 5

pthread_mutex_t mutex;
pthread_cond_t cond_votante;

struct urna{
    int counter_0;
    int counter_1;
} u;


int votanti;



void inizializza(void)
{
    pthread_mutexattr_t attributo_mutex;
    pthread_condattr_t attributo_cond;

    pthread_mutexattr_init(&attributo_mutex);
    pthread_condattr_init(&attributo_cond);

    pthread_mutex_init(&mutex, &attributo_mutex);
    pthread_cond_init(&cond_votante, &attributo_cond);
    

    pthread_mutexattr_destroy(&attributo_mutex);
    pthread_condattr_destroy(&attributo_cond);

    u.counter_0 = 0;
    u.counter_1 = 0;

    
    votanti = 0;

}


void vota(int v)
{
    pthread_mutex_lock(&mutex);
    if(v == 0){
        u.counter_0++;
    }
    else if(v == 1)
    {
        u.counter_1++;
    

    
}
votanti++;
pthread_mutex_unlock(&mutex);
}

int risultato(){

    while(votanti <= SOGLIA ){
        pthread_cond_wait(&cond_votante, &mutex);
    }
    if (&u.counter_0 > &u.counter_1){
    
    return 0;}
    else 
    {   
        return 1;}

   
}

void *thread(void *arg)
{
int voto = rand()%2;
vota(voto);
printf(arg);
pthread_mutex_lock(&mutex);
if (voto == risultato()) {printf("Ho vinto!\n");
}
else 
{printf("Ho perso!\n");
printf(arg);
}
pthread_mutex_unlock(&mutex);
pthread_exit(0);
}

int main()
{
    pthread_attr_t attr;
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;

    inizializza();

    pthread_attr_init(&attr);

  // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    

    pthread_create(&t1, &attr, thread, (char*)"Votante A: ");
    pthread_create(&t2, &attr, thread, (char*)"Votante B: ");
    pthread_create(&t3, &attr, thread, (char*)"Votante C: ");
    pthread_create(&t4, &attr, thread, (char*)"Votante D: ");
    pthread_create(&t5, &attr, thread, (char*)"Votante E: ");
    pthread_create(&t6, &attr, thread, (char*)"Votante F: ");
    pthread_create(&t7, &attr, thread, (char*)"Votante G: ");
    pthread_create(&t8, &attr, thread, (char*)"Votante H: ");
    pthread_create(&t9, &attr, thread, (char*)"Votante I: ");
    pthread_create(&t10, &attr, thread, (char*)"Votante J: ");
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    pthread_join(t6, NULL);
    pthread_join(t7, NULL);
    pthread_join(t8, NULL);
    pthread_join(t9, NULL);
    pthread_join(t10, NULL);
   



    return 0;

}
