#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

sem_t mutex;

sem_t sem_A;
sem_t sem_B;
sem_t sem_selector;
sem_t sem_ricevente;

int gruppo;
int num_messaggi;
char buffer[5];


void inizializza(void)
{
    
    sem_init(&mutex,0,1);
    sem_init(&sem_A,0,0);
    sem_init(&sem_B,0,0);
    srand(time(NULL));
    sem_init(&sem_ricevente,0,0);
    num_messaggi = 0;
    for(int i = 0; i<5; i++){
        buffer[i] = "nulla";
    }
}

void select_group(){
   gruppo = rand()%2;
   
}

void *MA0(){
    sem_wait(&mutex);
    if (gruppo == 1 && num_messaggi < 5){
        sem_post(&sem_A);

    }
    sem_post(&mutex);
    sem_wait(&sem_A);
    buffer[0] = "A";
    num_messaggi++;
    printf("0");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_A);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}

void *MA1(){
    sem_wait(&mutex);
    if (gruppo == 1 && num_messaggi < 5){
        sem_post(&sem_A);

    }
    sem_post(&mutex);
    sem_wait(&sem_A);
    buffer[1] = "A";
    num_messaggi++;
    printf("1");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_A);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}

void *MA2(){
    sem_wait(&mutex);
    if (gruppo == 1 &&  num_messaggi < 5){
        sem_post(&sem_A);

    }
    sem_post(&mutex);
    sem_wait(&sem_A);
    buffer[2] = "A";
    num_messaggi++;
    printf("2");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_A);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}

void *MA3(){
    sem_wait(&mutex);
    if (gruppo == 1 && num_messaggi < 5){
        sem_post(&sem_A);

    }
    sem_post(&mutex);
    sem_wait(&sem_A);
    buffer[3] = "A";
    num_messaggi++;
    printf("3");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_A);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}

void *MA4(){
    sem_wait(&mutex);
    if (gruppo == 1 && num_messaggi < 5){
        sem_post(&sem_A);

    }
    sem_post(&mutex);
    sem_wait(&sem_A);
    buffer[4] = "A";
    num_messaggi++;
    printf("4");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_A);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}

void *MB0(){
    sem_wait(&mutex);
    if (gruppo == 0 &&num_messaggi < 5){
        sem_post(&sem_B);

    }
    sem_post(&mutex);
    sem_wait(&sem_B);
    buffer[0] = "B";
    num_messaggi++;
    printf("0");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_B);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}

void *MB1(){
    sem_wait(&mutex);
    if (gruppo == 0 &&num_messaggi < 5){
        sem_post(&sem_B);

    }
    sem_post(&mutex);
    sem_wait(&sem_B);
    buffer[1] = "B";
    num_messaggi++;
    printf("1");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_B);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}
void *MB2(){
    sem_wait(&mutex);
    if (gruppo == 0 && num_messaggi < 5){
        sem_post(&sem_B);

    }
    sem_post(&mutex);
    sem_wait(&sem_B);
    buffer[2] = "B";
    num_messaggi++;
    printf("2");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_B);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}
void *MB3(){
    sem_wait(&mutex);
    if (gruppo == 0 && num_messaggi < 5){
        sem_post(&sem_B);

    }
    sem_post(&mutex);
    sem_wait(&sem_B);
    buffer[3] = "B";
    num_messaggi++;
    printf("3");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_B);
    }

    else if (num_messaggi == 5){
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}
void *MB4(){
    sem_wait(&mutex);
    if (gruppo == 0 &&num_messaggi < 5){
        sem_post(&sem_B);

    }
    sem_post(&mutex);
    sem_wait(&sem_B);
    buffer[4] = "B";
    num_messaggi++;
    printf("4");
    printf("\n");
    sem_wait(&mutex);
    if(num_messaggi < 5){
        
        sem_post(&sem_B);
    }

    else {
        
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    return 0;
    
}

void *ricevente(){
    sem_wait(&mutex);
    if(num_messaggi == 5){
        sem_post(&sem_ricevente);
    }
    sem_post(&mutex);
    sem_wait(&sem_ricevente);
    
    for(int i = 0; i < 5; i++){
        printf("%c", buffer[i]);
    }
    num_messaggi = 0;
    
}

int main()

{
    pthread_attr_t attr;
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
    pthread_t s,r;

   
    
    
    
    

    pthread_attr_init(&attr);

    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

     inizializza();
     select_group();

    
    
    pthread_create(&t1, &attr, MA0, NULL);
    pthread_create(&t2, &attr, MA1, NULL);
    pthread_create(&t3, &attr, MA2, NULL);
    pthread_create(&t4, &attr, MA3, NULL);
    pthread_create(&t5, &attr, MA4, NULL);
    pthread_create(&t6, &attr, MB0, NULL);
    pthread_create(&t7, &attr, MB1, NULL);
    pthread_create(&t8, &attr, MB2, NULL);
    pthread_create(&t9, &attr, MB3, NULL);
    pthread_create(&t10, &attr, MB4, NULL);
    pthread_create(&r, &attr, ricevente, NULL);
  

    

   // pthread_join(t1, NULL);
   // pthread_join(t2, NULL);
   // pthread_join(t3, NULL);
  //  pthread_join(t4, NULL);
   // pthread_join(t5, NULL);
   // pthread_join(t6, NULL);
   // pthread_join(t7, NULL);
  //  pthread_join(t8, NULL);
  //  pthread_join(t9, NULL);
   // pthread_join(t10, NULL);
  
    pthread_join(r, NULL);

    return 0;



    
}




