/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define CLIENTES 20
#define N_CAJAS 4

int caja_libre = -1; // cartel -1 indica libre y 0 ocupado.

sem_t cajeros, cola;
pthread_mutex_t m;

void * fun_cliente(void *ptr){
    int id = *((int*) ptr);
    printf("Cliente %d espera en la cola.\n", id);
    sem_wait(&cola);
    printf("Cliente %d accede a la caja: %d.\n", id, caja_libre);
    caja_libre = -1;
    sleep(1);
    printf("Cliente %d borra cartel: %d y avisa al siguiente.\n", id, caja_libre);
    sem_post(&cajeros);
    sleep(3);

}

void * fun_cajeros(void *ptr){
    int n = *((int*) ptr);
    while(1){
        sem_wait(&cajeros);
        printf("Cajero libre: %d.\n", n);
        caja_libre = n;
        sem_post(&cola);
        sleep(3);
    }

}         

void main(void){
    pthread_t th[CLIENTES], c[N_CAJAS];  /* identificadores de threads */
    int id[CLIENTES], n_caja[N_CAJAS];
    for(int i = 0; i < CLIENTES; i++){
        id[i] = i;
    }
    for(int i = 0; i < N_CAJAS; i++){
        n_caja[i] = i;
    }
    /* inicializar los semaforos */
    sem_init(&cajeros, 0, 1);
    sem_init(&cola, 0, 0);
    pthread_mutex_init(&m, NULL);
   /*crear los procesos ligeros */ 
    for(int i = 0; i < CLIENTES; i++){
        pthread_create(&th[i], NULL, fun_cliente, (void*)&id[i]);
    }
    for(int i = 0; i < N_CAJAS; i++){
        pthread_create(&c[i], NULL, fun_cajeros, (void*)&n_caja[i]);
    }   
    /*esperar su finalizacion */  
    for(int i = 0; i < CLIENTES; i++){
        pthread_join(th[i], NULL); 
    }
    for(int i = 0; i < N_CAJAS; i++){
        pthread_join(c[i], NULL); 
    }
}
 