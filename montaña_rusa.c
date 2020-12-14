/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define PASAJEROS 15 /* numero de clientes que van a querer cortarse el pelo. */
#define CAPACIDAD 4

int subidos = 0;

sem_t cap, coche;
pthread_mutex_t m;


void* fun_coche(void){
    while(1){
        pthread_mutex_lock(&m);
        if(subidos < CAPACIDAD){
            pthread_mutex_unlock(&m);
            printf("Coche esperando a que se suban...\n");
            sem_wait(&coche);
            printf("Comienza el viaje...\n");
        }
        else pthread_mutex_unlock(&m);
        
        sleep(3);
        printf("Acaba el viaje...\n");
        
        pthread_mutex_lock(&m);
        subidos = 0;
        pthread_mutex_unlock(&m);

        for(int i = 0; i < CAPACIDAD; i++)
            sem_post(&cap);
        printf("Se bajan todos los ocupantes.\n");
    }
}

void * fun_pasajeros(void *ptr){
    int id = *((int*) ptr);
    sem_wait(&cap);
   
    pthread_mutex_lock(&m);
    subidos++;
    printf("Se sube el pasajero %d.\n", id);
    if(subidos == CAPACIDAD){
        sem_post(&coche);
    }
    pthread_mutex_unlock(&m);
}           

void main(void){
    pthread_t th[PASAJEROS], th_coche;  /* identificadores de threads */
    /*inicializo el identificador de cada cliente para pasarlo a los threads. */
    int id[PASAJEROS];
    for(int i = 0; i < PASAJEROS; i++)
        id[i] = i;
    /* inicializar los semaforos */
    sem_init(&cap, 0, CAPACIDAD);
    sem_init(&coche, 0, 0);
    pthread_mutex_init(&m, NULL);

   /*crear los procesos ligeros */
    pthread_create(&th_coche, NULL, fun_coche, NULL);
    for(int i = 0; i < PASAJEROS; i++){
        pthread_create(&th[i], NULL, fun_pasajeros, (void*)&id[i]);
    }

    /*esperar su finalizacion */  
    for(int i = 0; i < PASAJEROS; i++)
        pthread_join(th[i], NULL);

    pthread_join(th_coche, NULL);
 }