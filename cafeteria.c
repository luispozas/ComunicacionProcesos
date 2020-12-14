/*Luis Pozas Palomo 01946200-D*/
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N_BOCADILLOS 2
#define CLIENTES 10

char* tipo[2] = {"jamon", "tortilla"};
int cant[2] = {N_BOCADILLOS, N_BOCADILLOS}; //inicialmente estan todos los tipos.

pthread_mutex_t m;
pthread_cond_t hay, no_hay;

int dormido = 1;

void* fun_clientes(void *ptr){
    int t = *((int*) ptr);
    while(1){
        pthread_mutex_lock(&m);
        while(cant[t] == 0){
            dormido = 0;
            pthread_cond_signal(&no_hay);
            pthread_cond_wait(&hay, &m);
        }
        printf("Cliente coge bocadillo de %s\n", tipo[t]);
        cant[t]--;
        pthread_mutex_unlock(&m); 
        sleep(3);
    }
}

void* fun_cocinero(){
    while(1){
        pthread_mutex_lock(&m);
        while(dormido){
            printf("cocinero se duerme.\n");
            pthread_cond_wait(&no_hay, &m);
        }
        printf("cocinero repone.\n");
        cant[0] = N_BOCADILLOS;
        cant[1] = N_BOCADILLOS;
        dormido = 1;
        sleep(3);
        pthread_cond_signal(&hay);
        pthread_mutex_unlock(&m);
    }
}

int main(){
    pthread_t c, s[CLIENTES];
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&hay, NULL);
    pthread_cond_init(&no_hay, NULL);

    int id[CLIENTES];
    for(int i = 0; i < CLIENTES; i++){
        id[i] = i%2;
    }

    pthread_create(&c, NULL, fun_cocinero, NULL);
    for(int i = 0; i < CLIENTES; i++){
        pthread_create(&s[i], NULL, fun_clientes, (void*) &id[i]);
    }
    
    while(1);
    return 0;
}