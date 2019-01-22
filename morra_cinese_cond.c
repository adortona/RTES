#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

pthread_cond_t cond_A;
pthread_cond_t cond_B;
pthread_cond_t cond_Arbitro;
pthread_mutex_t mutex;

int stato;

#define CARTA 0
#define SASSO 1
#define FORBICE 2

int mosse[3] = {CARTA, SASSO, FORBICE};

#define STATO_ZERO 0
#define ATTESA_MOSSE 1
#define MOSSA_TERMINATA 2
#define ATTESA_VALUTAZIONE 3

int scelta_A, scelta_B;

void inizializza(void)
{
    stato = STATO_ZERO;
    srand(time(NULL));
    pthread_mutexattr_t attributo_mutex;
    pthread_condattr_t attributo_cond;

    pthread_mutexattr_init(&attributo_mutex);
    pthread_condattr_init(&attributo_cond);

    pthread_mutex_init(&mutex, &attributo_mutex);
    pthread_cond_init(&cond_A, &attributo_cond);
    pthread_cond_init(&cond_B, &attributo_cond);
    pthread_cond_init(&cond_Arbitro, &attributo_cond);

    pthread_mutexattr_destroy(&attributo_mutex);
    pthread_condattr_destroy(&attributo_cond);

}

void *Arbitro(){
    pthread_mutex_lock(&mutex);
    if (stato == STATO_ZERO){
       printf("CARTA : 0");
       printf("\n");
       printf("SASSO : 1");
       printf("\n");
       printf("FORBICE: 2");
       printf("\n");
       printf("VIA! \n");
       printf("VIA! \n");
       stato = ATTESA_MOSSE;
       pthread_cond_signal(&cond_A);
       pthread_cond_signal(&cond_B);
       usleep(1000000);
       
        
        
        }
   
    while(stato != ATTESA_VALUTAZIONE){
        

        pthread_cond_wait(&cond_Arbitro, &mutex);

        if (scelta_A == CARTA && scelta_B == SASSO){
            printf("A VINCE");
        }

        else if (scelta_A == CARTA && scelta_B == SASSO){
            printf("A VINCE");
        }
        else if (scelta_A == CARTA && scelta_B == FORBICE){
            printf("b VINCE");
        }
        else if (scelta_A == FORBICE && scelta_B == SASSO){
            printf("B VINCE");
        }
        else if (scelta_A == FORBICE && scelta_B == CARTA){
            printf("A VINCE");
        }
        else if (scelta_A == SASSO && scelta_B == CARTA){
            printf("B VINCE");
        }
        else if (scelta_A == SASSO && scelta_B == FORBICE){
            printf("A VINCE");
        }
        else if (scelta_A == FORBICE && scelta_B == FORBICE){
            printf("PARI");
        }
        else if (scelta_A == CARTA && scelta_B == CARTA){
            printf("PARI");
        }
        else if (scelta_A == SASSO && scelta_B == SASSO){
            printf("PARI");
        }
        

        
    }

   
   pthread_mutex_unlock(&mutex);
   return 0;

}

void *giocatore_A(){
    pthread_mutex_lock(&mutex);
    while (stato != ATTESA_MOSSE && stato != MOSSA_TERMINATA){
        pthread_cond_wait(&cond_A, &mutex);
    }
    int n = rand() % 3;
    scelta_A = mosse[n];
    printf("A : %d \n",scelta_A);
    usleep(100000);
    
    if (stato == ATTESA_MOSSE){

        stato = MOSSA_TERMINATA;}

    else {
        stato = ATTESA_VALUTAZIONE;
        pthread_cond_signal(&cond_Arbitro);
       // pthread_cond_signal(&cond_Arbitro);
    }
    pthread_mutex_unlock(&mutex);
    
    
    return 0;
}

void *giocatore_B(){
    pthread_mutex_lock(&mutex);
    while (stato != ATTESA_MOSSE && stato != MOSSA_TERMINATA){
        pthread_cond_wait(&cond_B,&mutex);
    }
    int n = rand() % 2;
    scelta_B = mosse[n];
    
    printf("B : %d \n",scelta_B);
    usleep(100000);
    
    
    if (stato == ATTESA_MOSSE){

        stato = MOSSA_TERMINATA;
    }
    else {
        stato = ATTESA_VALUTAZIONE;
        pthread_cond_signal(&cond_Arbitro);
    }
    pthread_mutex_unlock(&mutex);
    
    
    return 0;
}

int main()
{
    pthread_attr_t attr;
    pthread_t t1, t2, t3;

    inizializza();
    
    

    pthread_attr_init(&attr);

    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    

    pthread_create(&t1, &attr, Arbitro, NULL);
    pthread_create(&t2, &attr, giocatore_A, NULL);
    pthread_create(&t3, &attr, giocatore_B, NULL);
  

    

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;



    
}