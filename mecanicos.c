/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N_EQUIPOS 1
#define CAP_M 30
#define CAP_L 10

int M[N_EQUIPOS];
int L[N_EQUIPOS];

pthread_mutex_t m[N_EQUIPOS];
pthread_cond_t m1[N_EQUIPOS], m2[N_EQUIPOS];

esperar_legada_coche(int equipo){
    sleep(5);
    printf("Llega el coche del equipo %d sin combustible.\n", equipo);
    L[equipo] = 0;
}


void mecanico1(void *ptr){
    int equipo = *((int*) ptr);;
    while(1){
        esperar_legada_coche(equipo);
        pthread_mutex_lock(&m[equipo]);
        while (M[equipo] < L[equipo]){
            printf("Mecanico 1 despierta al mecanico 2 (no queda combusitble en surtidor). equipo: %d\n", equipo);
            pthread_cond_signal(&m2[equipo]);
            printf("Mecanico 1 espera al mecanico 2 a que rellene surtidor. equipo: %d\n", equipo);
            pthread_cond_wait(&m1[equipo], &m[equipo]);
        }
        printf("Mecanico 1 rellena L al coche. equipo: %d\n", equipo);
        L[equipo] = CAP_L;
        M[equipo] -= CAP_L;
        pthread_mutex_unlock(&m[equipo]);
        sleep(1);
    }

}

void mecanico2(void *ptr){
    int equipo = *((int*) ptr);;
    while(1){
        pthread_mutex_lock(&m[equipo]);
        printf("Mecanico 2 espera al mecanico 1 a que le avise. equipo: %d\n", equipo);
        pthread_cond_wait(&m2[equipo], &m[equipo]);
        printf("Mecanico 2 es avisado por mecanico 1 y rellena surtidor. equipo: %d\n", equipo);
        M[equipo] = CAP_M;
        sleep(2);
        printf("Mecanico 2 avisa al mecanico 1 que el surtidor esta lleno. equipo: %d\n", equipo);
        pthread_cond_signal(&m1[equipo]);
        pthread_mutex_unlock(&m[equipo]);
    }
}

void main(void){
    pthread_t th[N_EQUIPOS*2];  /* identificadores de threads */
    int id[N_EQUIPOS];
    for(int i = 0; i < N_EQUIPOS; i++){
        id[i] = i;
        M[i] = CAP_M;
        L[i] = CAP_L;
    }

   /*crear los procesos ligeros */
    int i = 0;
    while(i < N_EQUIPOS*2){
        pthread_create(&th[i], NULL, mecanico1, (void*)&id[i]);
        i++;
        pthread_create(&th[i], NULL, mecanico2, (void*)&id[i-1]);
        i++;
    }
    for(int j = 0; j < N_EQUIPOS; j++){
        pthread_mutex_init(&m[i], NULL);
        pthread_cond_init(&m1[i], NULL);
        pthread_cond_init(&m2[i], NULL);
    }
    /*esperar su finalizacion */  
    for(int k = 0; k < N_EQUIPOS*2; k++){
        pthread_join(th[k], NULL); 
    }
}
 