#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

sem_t mutex;
sem_t barbieri;
sem_t divano;
sem_t cassiere;

#define SHAVING_ITERATIONS 999999999
#define PAYING_ITERATIONS 999999999

void inizializza(){

    sem_init(&mutex,0,1);
    sem_init(&barbieri,0,3);
    sem_init(&divano,0,4);
    sem_init(&cassiere,0,1);
}

void *cliente(int i){
    sem_wait(&mutex);
    printf("il cliente %d e' fuori la porta", i);
    printf("\n");
    sem_post(&mutex);
    sem_wait(&divano);
    sem_wait(&mutex);
    printf("il cliente %d e' seduto sul divano", i);
    printf("\n");
    sem_post(&divano);
    sem_post(&mutex);
    sem_wait(&barbieri);
    sem_wait(&mutex);
    printf("il cliente %d si sta tagliando i capelli", i);
    printf("\n");
    sem_post(&mutex);
    for (int j = 0; j < SHAVING_ITERATIONS; j++);
    sem_post(&barbieri);
    
    sem_wait(&cassiere);
    sem_wait(&mutex);
    printf("il cliente %d sta pagando", i);
    printf("\n");
    sem_post(&mutex);
    for (int j = 0; j < PAYING_ITERATIONS; j++);
    sem_post(&cassiere);
    printf("il cliente %d torna a casa", i);
    printf("\n");
    
    return 0;
}

int main()

{
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    pthread_attr_t attr;
    
    
    pthread_attr_init(&attr);

   

    inizializza();

    pthread_create(&t1, &attr, cliente, 1);
    pthread_create(&t2, &attr, cliente, 2);
    pthread_create(&t3, &attr, cliente, 3);
    pthread_create(&t4, &attr, cliente, 4);
    pthread_create(&t5, &attr, cliente, 5);
    pthread_create(&t6, &attr, cliente, 6);
    pthread_create(&t7, &attr, cliente, 7);
    pthread_create(&t8, &attr, cliente, 8);
    pthread_create(&t9, &attr, cliente, 9);
    pthread_create(&t10, &attr, cliente, 10);

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
