/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define CLIENTES 15 /* numero de clientes que van a querer cortarse el pelo. */
#define CAPACIDAD 5

int sentados = 0;

sem_t block;
pthread_mutex_t m;


void* fun_sushi(void){
    int i, n;
    pthread_mutex_lock(&m);
    if(comiendo == CAPACIDAD || esperando > 0){
        esperando++;
        pthread_mutex_unlock(&m);
        sem_wait(&block);
        sleep(3);
    }
    else{
        comiendo++;
        pthread_mutex_unlock(&m);
        sleep(3);
    }
    pthread_mutex_lock(&m);
    comiendo--;
    if(comiendo == 0 && esperando > 0){
        if(esperando < CAPACIDAD)
            n = esperando;
        else n = CAPACIDAD;
        esperando -= n;
        comiendo += n;
        for(int i = 0; i < n; ++i) sem_post(&block);
    }
    pthread_mutex_unlock(&m);
}


void main(void){
    pthread_t th[CLIENTES];  /* identificadores de threads */
    /*inicializo el identificador de cada cliente para pasarlo a los threads. */
    int id[CLIENTES];
    for(int i = 0; i < CLIENTES; i++)
        id[i] = i;
    /* inicializar los semaforos */
    sem_init(&block, 0, 0);
    pthread_mutex_init(&m, NULL);

   /*crear los procesos ligeros */
    for(int i = 0; i < CLIENTES; i++){
        pthread_create(&th[i], NULL, fun_sushi, NULL);
    }

    /*esperar su finalizacion */  
    for(int i = 0; i < CLIENTES; i++)
        pthread_join(th[i], NULL);
 }