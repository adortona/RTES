#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

pthread_mutex_t mutex;
pthread_cond_t cond_A;
pthread_cond_t cond_B;
pthread_cond_t cond_Reset;

int nA, nbA;
int nB, nbB;
int nReset, nbReset;

void inizializza(void)
{
    pthread_mutexattr_t attributo_mutex;
    pthread_condattr_t attributo_cond;

    pthread_mutexattr_init(&attributo_mutex);
    pthread_condattr_init(&attributo_cond);

    pthread_mutex_init(&mutex, &attributo_mutex);
    pthread_cond_init(&cond_A, &attributo_cond);
    pthread_cond_init(&cond_B, &attributo_cond);
    pthread_cond_init(&cond_Reset, &attributo_cond);

    pthread_mutexattr_destroy(&attributo_mutex);
    pthread_condattr_destroy(&attributo_cond);

    nA = nB = nReset = nbA = nbB = nbReset = 0;

}

void start_A(void){
    pthread_mutex_lock(&mutex);
    while(nReset > 0 || nA > 0 || nbReset > 0)
    {
        nbA++;
        pthread_cond_wait(&cond_A, &mutex);
        nbA--;
    }
    nA++;
    pthread_mutex_unlock(&mutex);
}

void end_A(void){
    pthread_mutex_lock(&mutex);
    nA--;
    if (nbReset > 0) {
        pthread_cond_signal(&cond_Reset);
    }

    else if (nbA > 0){
        pthread_cond_signal(&cond_A);
    }

    else
    {
        pthread_cond_signal(&cond_B);
    }

    pthread_mutex_unlock(&mutex);

}

void start_B(void){
    pthread_mutex_lock(&mutex);
     while(nReset > 0 || nB > 0 || nbReset > 0)
    {
        nbB++;
        pthread_cond_wait(&cond_B, &mutex);
        nbB--;
    }
    nB++;
    pthread_mutex_unlock(&mutex);
}

void end_B(void){
    pthread_mutex_lock(&mutex);
    nB--;
    if (nbReset > 0) {
        pthread_cond_signal(&cond_Reset);
    }

    else if (nbB > 0){
        pthread_cond_signal(&cond_B);
    }

     else
    {
        pthread_cond_signal(&cond_A);
    }

    pthread_mutex_unlock(&mutex);

}

void start_Reset(void){
    pthread_mutex_lock(&mutex);
     while(nA > 0 || nB > 0)
    {
        nbReset++;
        pthread_cond_wait(&cond_Reset, &mutex);
        nbReset--;
    }
    nReset++;
    pthread_mutex_unlock(&mutex);
}

void end_Reset(void){
    pthread_mutex_lock(&mutex);
    nReset--;
    if (nbA > 0) {
        pthread_cond_signal(&cond_A);
    }

    else if (nbB > 0){
        pthread_cond_signal(&cond_B);
    }

    pthread_mutex_unlock(&mutex);

}

void *A(void *arg){
    start_A();
    while(1){
        printf(arg);
        printf("\n");
    }
    end_A();
    return 0;
}

void *B(void *arg){
    start_B();
    while(1){
        printf(arg);
        printf("\n");
    }
    end_B();
    return 0;
}

void *Reset(void *arg){
    start_Reset();
    while(1){
        printf(" RESET \n");
    }
    end_Reset();
    return 0;
}


int main()
{
    pthread_attr_t attr;
    pthread_t t1, t2, t3;

    inizializza();

    pthread_attr_init(&attr);

    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    

    pthread_create(&t1, &attr, A, (char*)"A");
    pthread_create(&t1, &attr, A, (char*)"a");
    pthread_create(&t2, &attr, B, (char*)"B");
    pthread_create(&t2, &attr, B, (char*)"b");
    pthread_create(&t3, &attr, Reset, NULL);

    

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);



    return 0;




}