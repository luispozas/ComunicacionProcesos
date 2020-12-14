/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define SILLAS 2
#define CLIENTES 6 /* numero de clientes que van a querer cortarse el pelo. */

int n_ocupadas = 0;

sem_t cliente, barbero;
pthread_mutex_t m;

void* fun_barbero(void){
    while(1){
        pthread_mutex_lock(&m);
        if(n_ocupadas == 0){
            pthread_mutex_unlock(&m);
            sem_wait(&barbero);
            pthread_mutex_lock(&m);
            n_ocupadas--;
        }
        pthread_mutex_unlock(&m);
        printf("Barbero comienza.\n");
        sleep(3);
        sem_post(&cliente);
        sem_wait(&barbero);
        printf("Barbero acaba y avisa a un nuevo cliente.\n");
    }
}

void * fun_cliente (void *ptr){
    int id = *((int*) ptr);
    pthread_mutex_lock(&m);
    if(n_ocupadas <= SILLAS){
        n_ocupadas++;
        pthread_mutex_unlock(&m);
        printf("cliente %d en sala de espera.\n", id);
        sem_post(&barbero);
        sem_wait(&cliente);
        printf("Cliente %d acaba de cortarse el pelo.\n", id);
    }
    else {
        pthread_mutex_unlock(&m);
        printf("Cliente %d entra y ve que todo ocupado y se va.\n", id);
    }
}           

void main(void){
    pthread_t th[CLIENTES], th_barbero;  /* identificadores de threads */
    /*inicializo el identificador de cada cliente para pasarlo a los threads. */
    int id[CLIENTES];
    for(int i = 0; i < CLIENTES; i++)
        id[i] = i;
    /* inicializar los semaforos */
    sem_init(&cliente, 0, 0);
    sem_init(&barbero, 0, 0);
    pthread_mutex_init(&m, NULL);

   /*crear los procesos ligeros */
    pthread_create(&th_barbero, NULL, fun_barbero, NULL);
    for(int i = 0; i < CLIENTES; i++){
        pthread_create(&th[i], NULL, fun_cliente, (void*)&id[i]);
    }

    /*esperar su finalizacion */  
    for(int i = 0; i < CLIENTES; i++)
        pthread_join(th[i], NULL);

    pthread_join(th_barbero, NULL);
 }