/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CICLISTAS 20

char* modo_print[3] = {"izquierda", "derecha", "bidirecional"};

int cont[2] = {0, 0}; //inicialmente no hay ciclistas
int modo = 2; //inicialmente el modo es bidireccional.

pthread_mutex_t m;
pthread_cond_t espera, inanicicon;

int esperando[2] = {0, 0};

void cambia_modo(int sentido){
    pthread_mutex_lock(&m);
    if(modo != 2 && (!cont[0] && !cont[1] || (cont[0] && cont[1]) || (cont[0] <= 2 && !cont[1]) || (!cont[0] && cont[1] <= 2 ))){
        modo = 2;
        esperando[sentido] = 0;
        printf("Se cambia al modo BIDIRECCIONAL.\n" );
        pthread_cond_broadcast(&inanicicon);
        pthread_cond_broadcast(&espera);
    }
    else if(modo != 0 && cont[0] > 2 && !cont[1]){
        modo = 0;
        printf("Se cambia al modo IZQUIERDA.\n" );
        pthread_cond_broadcast(&espera);
    }
    else if(modo != 1 && cont[1] > 2 && !cont[0]){
        modo = 1;
        printf("Se cambia al modo DERECHA.\n" );
        pthread_cond_broadcast(&espera);
    }
    pthread_mutex_unlock(&m);
}

void acceder_al_puente(int sentido){
    pthread_mutex_lock(&m);
    while(modo != 2 && modo != sentido){
        esperando[sentido]++;
        printf("Ciclista esperando a entrar en sentido: %s\n", modo_print[sentido]);
        pthread_cond_wait(&espera, &m);
    }

    while(esperando[(sentido+1)%2]){
        printf("Ciclista espera a que los del sentido contrario(%s) pasen.\n", modo_print[(sentido+1)%2] );
        pthread_cond_wait(&inanicicon, &m);
    }

    cont[sentido]++;
    pthread_mutex_unlock(&m);
    printf("Ciclista sentido(%s) entra.\n", modo_print[sentido]);
    cambia_modo(sentido);
}
    

void salir_del_puente(int sentido){
    pthread_mutex_lock(&m);
    cont[sentido]--;
    pthread_mutex_unlock(&m);
    printf("Ciclista sale del puente sentido: %s\n", modo_print[sentido]);
    cambia_modo(sentido);
}

void* fun_ciclista(void *ptr){
    int sentido = *((int*) ptr);
    acceder_al_puente(sentido);
    sleep(4); //circular por el puente.
    salir_del_puente(sentido);
}

int main(){
    pthread_t s[N_CICLISTAS];
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&inanicicon, NULL);
    pthread_cond_init(&espera, NULL);

    int dir[N_CICLISTAS];
    for(int i = 0; i < N_CICLISTAS; i++){
        if(i < N_CICLISTAS/4) dir[i] = 0;
        else dir[i] = i%2;
    }
    for(int i = 0; i < N_CICLISTAS; i++){
        pthread_create(&s[i], NULL, fun_ciclista, (void*) &dir[i]);
        sleep(1);
    }
    while(1);
    return 0;
}