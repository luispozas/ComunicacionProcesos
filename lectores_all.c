//Luis Pozas Palomo codigo de Lectores y Escritores con semaforos
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int dato = 5;
int n_lect = 0;

sem_t s1, s2;
pthread_mutex_t m;

void* escritor(void){
	while(1){
		sem_wait(&s1);
		dato += 1;
		printf("escritor modifica dato a: %d\n", dato);
		sleep(1);
		sem_post(&s1);
	}
}

void* lector(){
	while(1){
		sleep(1);
		sem_wait(&s2);
		n_lect++;
		if(n_lect == 1) sem_wait(&s1);
		sem_post(&s2);

		printf("lector lee dato: %d\n", dato);
		sleep(5);

		sem_wait(&s2);
		n_lect--;
		if(n_lect == 0) sem_post(&s1);
		sem_post(&s2);
	}
}

int main(){
	pthread_t th1, th2, th3, th4;
	pthread_mutex_init(&m, NULL);
	sem_init(&s1, 0, 1);
	sem_init(&s2, 0, 1);

	pthread_create(&th1, NULL, lector, NULL);
	pthread_create(&th2, NULL, escritor, NULL);
	pthread_create(&th3, NULL, lector, NULL);
	pthread_create(&th4, NULL, escritor, NULL);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);

	pthread_mutex_destroy(&m);

	return 0;
}

/*
//Luis Pozas Palomo codigo de Lectores y Escritores con mutex y variables condicionales
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int dato = 5;
int n_lect = 0;

pthread_mutex_t m;

pthread_cond_t l;

void* escritor(){
	while(1){
		pthread_mutex_lock(&m);
		while(n_lect > 0) pthread_cond_wait(&l, &m);
		dato += 1;
		printf("escritor modifica dato a: %d\n", dato);
		sleep(1);
		pthread_mutex_unlock(&m);
	}
}

void* lector(){
	while(1){
		sleep(1);
		pthread_mutex_lock(&m);
		n_lect++;
		pthread_mutex_unlock(&m);

		printf("lector lee dato: %d\n", dato);
		sleep(5);

		pthread_mutex_lock(&m);
		n_lect--;
		if(n_lect == 0) pthread_cond_signal(&l);
		pthread_mutex_unlock(&m);
	}
}

int main(){
	pthread_t th1, th2, th3, th4;
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&l, NULL);

	pthread_create(&th1, NULL, lector, NULL);
	pthread_create(&th2, NULL, escritor, NULL);
	pthread_create(&th3, NULL, lector, NULL);
	pthread_create(&th4, NULL, escritor, NULL);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);

	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&l);

	return 0;
}
*/

/*
//Luis Pozas Palomo codigo de Lectores y Escritores con mutex
#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int dato = 5;
int n_lect = 0;

pthread_mutex_t m_dato;
pthread_mutex_t m_lect;


void* escritor(void){
	while(1){
		pthread_mutex_lock(&m_dato);
		dato += 1;
		printf("escritor modifica dato a: %d\n", dato);
		sleep(1);
		pthread_mutex_unlock(&m_dato);
	}
}

void* lector(void){
	while(1){
		sleep(1);
		pthread_mutex_lock(&m_lect);
		n_lect++;
		if(n_lect == 1)
			pthread_mutex_lock(&m_dato);
		pthread_mutex_unlock(&m_lect);

		printf("lector lee dato: %d\n", dato);
		sleep(5);

		pthread_mutex_lock(&m_lect);
		n_lect--;
		if(n_lect == 0)
			pthread_mutex_unlock(&m_dato);
		pthread_mutex_unlock(&m_lect);
	}
}

int main(){
	pthread_t th1, th2, th3, th4;
	pthread_mutex_init(&m_dato, NULL);
	pthread_mutex_init(&m_lect, NULL);

	pthread_create(&th1, NULL, lector, NULL);
	pthread_create(&th2, NULL, escritor, NULL);
	pthread_create(&th3, NULL, lector, NULL);
	pthread_create(&th4, NULL, escritor, NULL);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);

	pthread_mutex_destroy(&m_lect);
	pthread_mutex_destroy(&m_dato);

	return 0;
}

*/
