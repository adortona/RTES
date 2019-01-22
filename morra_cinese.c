#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

sem_t mutex;

sem_t sem_A;
sem_t sem_B;
sem_t sem_Arbitro;

int stato;

#define CARTA 8
#define SASSO 9
#define FORBICE 10

int mosse[3] = {CARTA, SASSO, FORBICE};

#define STATO_ZERO 0
#define ATTESA_MOSSA_A 1
#define ATTESA_MOSSA_B 2
#define ATTESA_VALUTAZIONE 3

int scelta_A, scelta_B;

void inizializza(void)
{
    srand(time(NULL));
    sem_init(&mutex,0,1);
    sem_init(&sem_A,0,0);
    sem_init(&sem_B,0,0);
    sem_init(&sem_Arbitro,0,0);
    stato = STATO_ZERO;
}

void *Arbitro(){
    sem_wait(&mutex);
    if (stato == STATO_ZERO){
       stato = ATTESA_MOSSA_A;
       printf("CARTA : 8");
       printf("\n");
       printf("SASSO : 9");
       printf("\n");
       printf("FORBICE: 10");
       printf("\n");
       printf("VIA! \n");
       usleep(1000000);
        sem_post(&sem_A);
        
        }
    sem_post(&mutex);
    sem_wait(&sem_Arbitro);
    sem_wait(&mutex);
    if (stato == ATTESA_VALUTAZIONE){
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

   
   sem_post(&mutex);
   return 0;

}

void *giocatore_A(){
    sem_wait(&mutex);
    if(stato == ATTESA_MOSSA_A){
        sem_post(&sem_A);
    }
    else stato = STATO_ZERO;
    sem_post(&mutex);
    sem_wait(&sem_A);
    
    int n = rand()%3;
    scelta_A = mosse[n];
    printf("A : %d \n",scelta_A);
    //usleep(10000);
    sem_wait(&mutex);
    stato = ATTESA_MOSSA_B;
    sem_post(&sem_B);
    sem_post(&mutex);
    return 0;
}

void *giocatore_B(){
    sem_wait(&mutex);
    if(stato == ATTESA_MOSSA_B){
        sem_post(&sem_B);
    }
    else stato = STATO_ZERO;
    sem_post(&mutex);
    sem_wait(&sem_B);
    int j = rand()%3;
    scelta_B = mosse[j];
    printf("B : %d \n",scelta_B);
    //usleep(100000);
    sem_wait(&mutex);
    stato = ATTESA_VALUTAZIONE;
    sem_post(&sem_Arbitro);
    sem_post(&mutex);
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