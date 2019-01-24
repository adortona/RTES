#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define N 10
#define NESSUNA_PIZZA 0
#define PIZZE_PENDENTI 1

#define ATTESA 3

struct codaclienti_t{

    sem_t mutex;
    sem_t sem_pizzaiolo;
    sem_t sem_cliente[N];
    int turno;
    int state;
    int conta_pizza;
    int ordine_clienti[N];
} coda;

void init_codaclienti(struct codaclienti_t *codaclienti){

    sem_init(&codaclienti->mutex,0,1);
    sem_init(&codaclienti->sem_pizzaiolo,0,0);
    codaclienti->turno = 0;
    codaclienti->state = NESSUNA_PIZZA;
    codaclienti->conta_pizza = 0;
    for(int i = 0; i < N; i++){ // virgole al posto di punti e virgola
       codaclienti->ordine_clienti[i] = 0; //C'era & di troppo
    }
    for(int i = 0; i < N; i++){
        sem_init(&codaclienti->sem_cliente[i],0,0);
    }
}

void ordina_pizze(struct codaclienti_t *codaclienti, int numeropizze, int cliente){

    int min = 100;
    sem_wait(&codaclienti->mutex);
    codaclienti->state = PIZZE_PENDENTI;
    codaclienti->ordine_clienti[cliente] = numeropizze;
    printf("il cliente %d ordina %d pizze \n",cliente,numeropizze);
    for (int i = 0; i < N; i++){
        if(min > codaclienti->ordine_clienti[i]){
            min = codaclienti->ordine_clienti[i];
            codaclienti->turno = i;
    }
    }
    // NON HO RILASCIATO UN MUTEX
    sem_post(&codaclienti->mutex);
    sem_post(&codaclienti->sem_pizzaiolo);

}

void prossima_pizza(struct codaclienti_t *codaclienti){

    sem_wait(&codaclienti->mutex);
    if (codaclienti->state == PIZZE_PENDENTI){  // dimenticato codaclienti->
       sem_post(&codaclienti->sem_pizzaiolo);
     } 
    sem_post(&codaclienti->mutex);
    sem_wait(&codaclienti->sem_pizzaiolo);
    
}

void consegna_pizza(struct codaclienti_t *codaclienti){

    sem_wait(&codaclienti->mutex);
    codaclienti->conta_pizza++;
    sem_post(&codaclienti->mutex);
    if (codaclienti->ordine_clienti[codaclienti->turno] == codaclienti->conta_pizza){ // & di troppo e mancava un codaclienti
       sem_post(&codaclienti->sem_cliente[codaclienti->turno]);
    }
}

void ritira_pizze(struct  codaclienti_t *codaclienti, int cliente){
    int min = 100;
    sem_wait(&codaclienti->mutex);
    //& di troppo
    if(codaclienti->ordine_clienti[cliente] == codaclienti->conta_pizza && codaclienti->turno == cliente){
        sem_post(&codaclienti->sem_cliente[cliente]);
    }
    sem_post(&codaclienti->mutex);
    sem_wait(&codaclienti->sem_cliente[cliente]);
    codaclienti->conta_pizza = 0;

    //----parte aggiunta------
    for (int i = 0; i < N; i++){
        if(min > codaclienti->ordine_clienti[i]){
          min = codaclienti->ordine_clienti[i];
          codaclienti->turno = i;
          
    }
    sem_post(&codaclienti->sem_cliente[codaclienti->turno]);
}   
    //----fine parte aggiunta
}

void *cliente(void *arg){

   while(1){
        sleep(ATTESA);
        printf("il cliente %d entra in pizzeria \n", arg);
        sleep(ATTESA);
        int pizze = rand()%6;
        ordina_pizze(&coda, pizze, (int*)arg);
        sleep(ATTESA);
        
        ritira_pizze(&coda, (int*)arg);
        printf("il cliente %d torna a casa \n", arg);
        sleep(ATTESA);
       // sleep(1);
    }
    return 0;
}

void *pizzaiolo(void *arg){

    while(1){
        
        prossima_pizza(&coda);
        printf("impasto e cottura \n");
        consegna_pizza(&coda);
        
    }
    return 0;
}


int main(){

    pthread_attr_t attr;
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    pthread_t pizzaiolo;
    srand(time(NULL));
    init_codaclienti(&coda);

    pthread_attr_init(&attr);
    
    pthread_create(&t10, &attr, cliente, (int*)0);
    pthread_create(&t1, &attr, cliente, (int*)1);
    pthread_create(&t2, &attr, cliente, (int*)2);
    pthread_create(&t3, &attr, cliente, (int*)3);
    pthread_create(&t4, &attr, cliente, (int*)4);
    pthread_create(&t5, &attr, cliente, (int*)5);
    pthread_create(&t6, &attr, cliente, (int*)6);
    pthread_create(&t7, &attr, cliente, (int*)7);
    pthread_create(&t8, &attr, cliente, (int*)8);
    pthread_create(&t9, &attr, cliente, (int*)9);
    pthread_create(&pizzaiolo, &attr, cliente, NULL);


    

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
    pthread_join(pizzaiolo, NULL);

    
   
    return 0;


}