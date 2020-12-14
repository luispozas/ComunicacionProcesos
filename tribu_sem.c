/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N_SALVAJES 4
#define N_RACIONES_CALDERO 5

sem_t raciones_caldero, raciones_salvajes;
int n_raciones = N_RACIONES_CALDERO;

int espera = 1;

void* salvaje(){
    while(1){
        sem_wait(&raciones_caldero);
        n_raciones -=1;
        printf("Salvaje se come una racion del caldero, quedan: %d\n", n_raciones);
        sleep(1);
        if(n_raciones == 1) espera = 0;
    }
}

void* cocinero(){
    while(1){
        while(espera);
        sem_wait(&raciones_salvajes);
        printf("Cocinero recarga el caldero\n");
        sleep(3); //accion de ponerlas en la cazuela
        n_raciones = N_RACIONES_CALDERO;
        for(int i = 0; i < N_RACIONES_CALDERO; i++)
            sem_post(&raciones_caldero);
        espera = 0;
        sem_post(&raciones_salvajes);
        
    }
}

int main(){
    pthread_t c, s[4];
    sem_init(&raciones_caldero, 0, N_RACIONES_CALDERO);
    sem_init(&raciones_salvajes, 0, 1);
    pthread_create(&c, NULL, cocinero, NULL);
    for(int i = 0; i < N_SALVAJES; i++)
        pthread_create(&s[i], NULL, salvaje, NULL);
    while(1);
    return 0;
}