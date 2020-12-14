/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

char *ing[]={"tabaco", "papel", "cerilla"};
int nuevosElem[2];
pthread_mutex_t mutex;
pthread_cond_t lleno, turno;
int completo = 0;
int siguiente = 0;



void* agente(void){
    int i, j, k=0;
    while(1){
        pthread_mutex_lock(&mutex);
        while(completo) //si la mesa esta llena de cosas
            pthread_cond_wait(&lleno, &mutex);
        i = k; j = (i+1)%3; k = j;
        nuevosElem[0] = i;
        nuevosElem[1] = (i+1)%3;
        completo = 1;
        siguiente = 1;
        printf("Agente poniendo elementos %s, y %s en la mesa\n", ing[i], ing[(i+1)%3]);
        sleep(1);
        pthread_cond_broadcast(&turno);
        pthread_mutex_unlock(&mutex);
    }
}

void* fumador(void *ptr){
    int i = *((int*) ptr);
    while(1){
        pthread_mutex_lock(&mutex);
        while(!siguiente || nuevosElem[0] == i || nuevosElem[1] == i)
            pthread_cond_wait(&turno, &mutex);
        printf("Fumador con elemento %s coge los elementos %s, y %s de la mesa y fuma.\n", ing[i], ing[nuevosElem[0]], ing[nuevosElem[1]]);
        sleep(3);
        completo = 0;
        siguiente = 0;
        pthread_cond_signal(&lleno);
        pthread_mutex_unlock(&mutex);

    }
}

int main(){
    int i[3] = {0, 1, 2};
    pthread_t ag, f0, f1, f2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&lleno, NULL);
    pthread_cond_init(&turno, NULL);
    pthread_create(&ag, NULL, agente, NULL);
    pthread_create(&f0, NULL, fumador, (void*) &i[0]);
    pthread_create(&f1, NULL, fumador, (void*) &i[1]);
    pthread_create(&f2, NULL, fumador, (void*) &i[2]);
    while(1);
    return 0;
}