#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N 4

sem_t sem_mutex, sem_barrera;
int count = 0;

void* fun(){
	sem_wait(&sem_mutex);
	count++;
	sem_post(&sem_mutex);
	if(count == N){
		for(int i = 0; i < N; i++) sem_post(&sem_barrera);
		printf("Ultimo.\n");
	}
	printf("Espera hilo a que terminen todos.\n");
	sem_wait(&sem_barrera);
	printf("Pasa hilo.\n");
}

int main(){
	pthread_t th1, th2, th3, th4;
	sem_init(&sem_mutex, 0, 1);
	sem_init(&sem_barrera, 0, 0);

	pthread_create(&th1, NULL, fun, NULL);
	pthread_create(&th2, NULL, fun, NULL);
	pthread_create(&th3, NULL, fun, NULL);
	pthread_create(&th4, NULL, fun, NULL);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);

	return 0;
}