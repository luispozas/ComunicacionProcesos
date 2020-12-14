/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define EQUIPOS 8
#define MIEMBROS 4

#define n 32

pthread_mutex_t m;
pthread_cond_t varC[n], barrera;

int listos = 0;
int turno = 0;

void * fun_nadador(void *ptr){
    int id = *((int*) ptr);
    int team = id%EQUIPOS;

    pthread_mutex_lock(&m);
    while(id%4 != turno){
        printf("Nadador %d se posiciona en la cola.\n", id);
        pthread_cond_wait(&varC[id], &m);
    }
    pthread_mutex_unlock(&m);

    printf("Nadador %d comienza la carrera.\n", id);
    sleep(5);
    printf("Nadador %d acaba la carrera.\n", id);

    pthread_mutex_lock(&m);
    listos++;
    while(listos != EQUIPOS){
        printf("Nadador %d espera a que terminen la carrera. listos: %d.\n", id, listos);
        pthread_cond_wait(&barrera, &m);
    }
    pthread_cond_broadcast(&barrera);
    turno++;
    listos = 0;
    printf("Turno %d.\n", turno);
    for(int i = 0; i < n; i++){
        pthread_cond_signal(&varC[i]);
    }
    pthread_mutex_unlock(&m);
}           

void main(void){
    pthread_t th[n];  /* identificadores de threads */
    int id[n];
    for(int i = 0; i < n; i++){
        id[i] = i;
    }

    pthread_mutex_init(&m, NULL);
    for(int i = 0; i < n; i++){
        pthread_cond_init(&varC[i], NULL);
    }
    
   /*crear los procesos ligeros */ 
    for(int i = 0; i < n; i++){
        pthread_create(&th[i], NULL, fun_nadador, (void*)&id[i]);
    }   
    /*esperar su finalizacion */  
    for(int i = 0; i < n; i++){
        pthread_join(th[i], NULL); 
    }
}
 