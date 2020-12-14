/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N_SALVAJES 4
#define N_RACIONES_CALDERO 5

pthread_mutex_t mutex;
pthread_cond_t lleno, vacio;
int n_raciones = 0;


void* salvaje(){
    while(1){
        pthread_mutex_lock(&mutex);
        while(n_raciones == 0) //si la mesa esta llena de cosas
            pthread_cond_wait(&vacio, &mutex);
        n_raciones -= 1;
        printf("Salvaje se come una racion del caldero\n");
        sleep(1);
        if(n_raciones == 0) pthread_cond_signal(&lleno);
        pthread_mutex_unlock(&mutex);
    }
}

void* cocinero(){
    while(1){
        pthread_mutex_lock(&mutex);
        while(n_raciones == N_RACIONES_CALDERO)
            pthread_cond_wait(&lleno, &mutex);
        printf("Cocinero recarga una racion en el caldero\n");
        sleep(3); //accion de ponerlas en la cazuela
        n_raciones += 1;
        pthread_cond_signal(&vacio);
        pthread_mutex_unlock(&mutex);
    }
}

int main(){
    pthread_t c, s[4];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&lleno, NULL);
    pthread_cond_init(&vacio, NULL);
    pthread_create(&c, NULL, cocinero, NULL);
    for(int i = 0; i < N_SALVAJES; i++)
        pthread_create(&s[i], NULL, salvaje, NULL);
    while(1);
    return 0;
}