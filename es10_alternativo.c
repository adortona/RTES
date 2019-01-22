#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define DELAY 999999999999


struct gestore{ 

  sem_t sem_A;
  sem_t sem_B;
  sem_t mutex;
  
  int nA;
  int nB;
  int nQ;
  int n2;
  int nbA;
  int nbB;
  int nbQ;
  int nb2;

  int priorita_attuale;

} gest; 


void inizializza(struct gestore *g){

    sem_init(&g->sem_A,0,1);
    sem_init(&g->sem_B,0,1);
    
    srand(time(NULL))
}

void *RicQ(struct gestore *g){
    
    priorita_attuale = 1;


    sem_wait(&g->mutex);
    int a = rand()%2;
    if( a == 0){
        g->nQ++;
     
        sem_wait(&g->sem_A);
        printf("Sto acquisendo la risorsa generica (A) \n");
        sleep(2);
     
        g->nQ--;
        printf("Sto rilasciando la risorsa generica (A) \n");
        sem_post(&g->sem_A);
        sleep(2);
      
    }
    else if(a == 1){
        g->nQ++;
     
        sem_wait(&g->sem_B);
        printf("Sto acquisendo la risorsa generica (B) \n");
        sleep(2);
      
        g->nQ--;
        printf("Sto rilasciando la risorsa generica (B) \n");
        sem_post(&g->sem_B);
        sleep(2);
        sem_post(&g->mutex);
    }
    else {sem_post(&g->mutex);}
    return 0;
    

}

void *RicA(struct gestore *g){
   
   
  
   sem_wait(&g->sem_A);
   printf("Ho acquisito la risorsa A \n");
   sleep(2);
  
   printf("ho rilasciato la risorsa A \n");
   sleep(2);
   sem_post(&g->sem_A);
   return 0;
   
}

void *RicB(struct gestore *g){
  
  
   sem_wait(&g->sem_B);
   printf("Ho acquisito la risorsa B \n");
   sleep(2);
  
   printf("ho rilasciato la risorsa B \n");
   sleep(2);
   sem_post(&g->sem_B);
   return 0;
    
}

void *Ric2(struct gestore *g){
 
 
   sem_wait(&g->sem_A);
   sem_wait(&g->sem_B);
   printf("Ho acquisito entrambe le risorse \n");
   sleep(2);
   
   printf("ho rilasciato entrambe le risorse \n");
   sleep(2);
 
   sem_post(&g->sem_A);
   sem_post(&g->sem_B);
   return 0;
    
    
}

int main(){

 pthread_attr_t attr;
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    pthread_t s,r;

   
    
    
    
    

    pthread_attr_init(&attr);

   

     inizializza(&gest);
     

    
    
    pthread_create(&t1, &attr, RicA, &gest);
    pthread_create(&t2, &attr, Ric2, &gest);
    pthread_create(&t3, &attr, RicQ, &gest);
    pthread_create(&t4, &attr, RicA, &gest);
    pthread_create(&t5, &attr, RicB, &gest);
    pthread_create(&t6, &attr, RicA, &gest);
    pthread_create(&t7, &attr, RicA, &gest);
    pthread_create(&t8, &attr, RicB, &gest);
    pthread_create(&t9, &attr, Ric2, &gest);
    pthread_create(&t10, &attr, RicQ, &gest);

    pthread_join (t1, NULL);
    pthread_join (t2, NULL);
    pthread_join (t3, NULL);
    pthread_join (t4, NULL);
    pthread_join (t5, NULL);
    pthread_join (t6, NULL);
    pthread_join (t7, NULL);
    pthread_join (t8, NULL);
    pthread_join (t9, NULL);
    pthread_join (t10, NULL);
    
    
    return 0;



    

}