/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define CLIENTES 10

int s_libre[2] = {0, 0}; //0 indica libre y 1 ocupado.

sem_t s1;
pthread_mutex_t m;

void * cliente(void *ptr){
    int id = *((int*) ptr);
    int surtidor;
    sem_wait(&s1);
    pthread_mutex_lock(&m);
    if(s_libre[0] == 1 && s_libre[1] == 0){ // si surtidor 0 esta ocupado y el 1 libre...
        s_libre[1] = 1; surtidor = 1;
    }
    else { s_libre[0] = 1; surtidor = 0;}
    pthread_mutex_unlock(&m);
    printf(" %d Cliente comienza en surtidor %d. \n", id, surtidor+1);
    sleep(6); //sustituye a la funcion servir_combustible(servidor, dinero);
    printf(" %d Cliente acaba en surtidor %d. \n", id, surtidor+1);
    s_libre[surtidor] = 0;
    sem_post(&s1);
}           

void main(void){
    pthread_t th[CLIENTES];  /* identificadores de threads */
    int id[CLIENTES];
    for(int i = 0; i < CLIENTES; i++){
        id[i] = i;
    }
    /* inicializar los semaforos */
    sem_init(&s1, 0, 2);
    pthread_mutex_init(&m, NULL);
   /*crear los procesos ligeros */ 
    for(int i = 0; i < CLIENTES; i++){
        pthread_create(&th[i], NULL, cliente, (void*)&id[i]);
    }   
    /*esperar su finalizacion */  
    for(int i = 0; i < CLIENTES; i++){
        pthread_join(th[i], NULL); 
    }
}
 