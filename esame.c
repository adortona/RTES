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
    codaclienti->turno = 10;
    codaclienti->state = NESSUNA_PIZZA;
    codaclienti->conta_pizza = 0;
    for(int i = 0; i < N; i++){ // virgole al posto di punti e virgola
       codaclienti->ordine_clienti[i] = 100; //C'era & di troppo
    }
    for(int i = 0; i < N; i++){
        sem_init(&codaclienti->sem_cliente[i],0,0);
    }
}

void ordina_pizze(struct codaclienti_t *codaclienti, int numeropizze, int cliente){

   
    sem_wait(&codaclienti->mutex);
    codaclienti->state == PIZZE_PENDENTI;
    codaclienti->ordine_clienti[cliente] = numeropizze;
    printf("il cliente %d ordina %d pizze \n \n",cliente,numeropizze);
    if(codaclienti->ordine_clienti[codaclienti->turno] > codaclienti->ordine_clienti[cliente] || codaclienti->turno > 9){
       codaclienti->turno = cliente;
       printf("E' IL TURNO DI %d \n \n", cliente);
    }
    
    // NON HO RILASCIATO UN MUTEX--------------
    sem_post(&codaclienti->mutex);
    // ----MUTEX NON RISLASCIATO SOPRA----------
    
    
    
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
    
    sem_wait(&codaclienti->mutex);
    //& di troppo
    if(codaclienti->ordine_clienti[cliente] == codaclienti->conta_pizza && codaclienti->turno == cliente){ //IL DEADLOCK È NELLA SECONDA CONDIZIONE
        sem_post(&codaclienti->sem_cliente[cliente]);
    }
    sem_post(&codaclienti->mutex);
    sem_wait(&codaclienti->sem_cliente[cliente]);
    sem_wait(&codaclienti->mutex);
    printf("il cliente %d torna a casa \n \n", cliente);
    codaclienti->conta_pizza = 0;
    codaclienti->ordine_clienti[cliente] = 100; // RIGA AGGIUNTA PER AZZERRARE LE PIZZE
    //----parte aggiunta--------------------------------------------------------------------------
    
    for (int i = 0; i < N; i++){
        if(codaclienti->ordine_clienti[codaclienti->turno] > codaclienti->ordine_clienti[i]){
            codaclienti->turno = i;
            printf("E' IL TURNO DI %d \n \n",i);
    }
    
    }
    sem_post(&codaclienti->mutex);      
    sem_post(&codaclienti->sem_cliente[codaclienti->turno]);

    //----fine parte aggiunta-----------------------------------------------------------------------
}   
    


void *cliente(void *arg){

   while(1){
        
        int pizze = rand()% 10;
        ordina_pizze(&coda, pizze, (int*)arg);
        sleep(ATTESA);
        ritira_pizze(&coda, (int*)arg);
        }
    return 0;
}

void *pizzaiolo(void *arg){

    while(1){

        prossima_pizza(&coda);
        consegna_pizza(&coda);
        }
    return 0;
}


int main(){

    pthread_attr_t attr;
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
    
    srand(time(NULL));
    init_codaclienti(&coda);

    pthread_attr_init(&attr);
    
    
    
    sleep(ATTESA);
    pthread_create(&t1, &attr, cliente, (int*)1);
    sleep(ATTESA);
    pthread_create(&t2, &attr, cliente, (int*)2);
    sleep(ATTESA);
    pthread_create(&t3, &attr, cliente, (int*)3);
    sleep(ATTESA);
    pthread_create(&t4, &attr, cliente, (int*)4);
    sleep(ATTESA);
    pthread_create(&t5, &attr, cliente, (int*)5);
    sleep(ATTESA);
    pthread_create(&t6, &attr, cliente, (int*)6);
    sleep(ATTESA);
    pthread_create(&t7, &attr, cliente, (int*)7);
    sleep(ATTESA);
    pthread_create(&t8, &attr, cliente, (int*)8);
    sleep(ATTESA);
    pthread_create(&t9, &attr, cliente, (int*)9);
    sleep(ATTESA);
    pthread_create(&t10, &attr, cliente, (int*)0);
    sleep(ATTESA);
    pthread_create(&t11, &attr, pizzaiolo, NULL);


    
    
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
    pthread_join(t11, NULL);

    
   
    return 0;


}