//--------------------------------------------------------------------------------------------------------------------------

//Luis Pozas Palomo codigo de Productor/consumidor con semafoross
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_BUFFER 5
#define DATOS_A_PRODUCIR 100000

sem_t huecos, elementos;

int buffer[MAX_BUFFER];

void* consumidor(){
	int dato, pos = 0;
	for(int i = 0; i < DATOS_A_PRODUCIR; i++){
		sem_wait(&elementos);
		dato = buffer[pos];
		pos = (pos++) % MAX_BUFFER;
		printf("consume\n");
		sleep(1);
		sem_post(&huecos);
	}
}


void* productor(){
	int dato, pos = 0;
	for(int i = 0; i < DATOS_A_PRODUCIR; i++){
		sem_wait(&huecos);
		buffer[pos] = 1;
		pos = (pos++) % MAX_BUFFER;
		printf("produce\n");
		sleep(1);
		sem_post(&elementos);
	}
}

void main(){
	pthread_t th1, th2;

	sem_init(&huecos, 0, MAX_BUFFER);
	sem_init(&elementos, 0, 0);

	pthread_create(&th1, NULL, productor, NULL);
	pthread_create(&th2, NULL, consumidor, NULL);

	while(1);
}


//--------------------------------------------------------------------------------------------------------------------------

/*
//Luis Pozas Palomo codigo de Productor/consumidor con variables condicionales
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_BUFFER 5
#define DATOS_A_PRODUCIR 100000

pthread_mutex_t m;
pthread_cond_t lleno, vacio;
int n_elem;
int buffer[MAX_BUFFER];

void* consumidor(){
	int dato, pos = 0;
	for(int i = 0; i < DATOS_A_PRODUCIR; i++){
		pthread_mutex_lock(&m);
		while(n_elem == 0)
			pthread_cond_wait(&vacio, &m);
		dato = buffer[pos];
		printf("Numero de elementos consumidos: %d\n", n_elem - 1);
		sleep(1);
		pos = (pos++) % MAX_BUFFER;
		n_elem--;
		pthread_cond_signal(&lleno); //no esta lleno -> se puede producir
		pthread_mutex_unlock(&m);
	}
}


void* productor(){
	int dato, pos = 0;
	for(int i = 0; i < DATOS_A_PRODUCIR; i++){
		dato = 1; //supongamos que los datos que se producen son siempre 1;
		pthread_mutex_lock(&m);
		while(n_elem == MAX_BUFFER)
			pthread_cond_wait(&lleno, &m);
		buffer[pos] = dato;
		printf("Numero de elementos producidos: %d\n", n_elem + 1);
		sleep(1);
		pos = (pos++) % MAX_BUFFER;
		n_elem++;
		pthread_cond_signal(&vacio); //no esta vacia -> se puede consumir
		pthread_mutex_unlock(&m);
	}
}

void main(){
	pthread_t th1, th2;

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&lleno, NULL);
	pthread_cond_init(&vacio, NULL);

	pthread_create(&th1, NULL, productor, NULL);
	pthread_create(&th2, NULL, consumidor, NULL);

	while(1);
}
*/