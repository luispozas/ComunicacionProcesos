/*Luis Pozas Palomo 01946200-D, Ming yang Chen x9805942F*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define EN_RUTA    0   // autobus en ruta
#define EN_PARADA  1   // autobus en la parada

int MAX_USUARIOS = 40; // capacidad del autobús
int N_PARADAS = 5;     // numero de paradas de la ruta
int USUARIOS = 4;      // numero de usuarios

// estado inicial
int estado        = EN_RUTA;
int parada_actual = 0;     // parada en la que se encuentra el autobus
int n_ocupantes   = 0;     // ocupantes que tiene el autobús
// personas que desean subir en cada parada
int *esperando_parada;
// personas que desean bajar en cada parada
int *esperando_bajar;

// Otras definiciones globales (comunicacion y sincronizacion)
pthread_mutex_t mutex;

pthread_t *th_usuario, th_bus;

pthread_cond_t en_parada;
pthread_cond_t subir;
pthread_cond_t bajar;


void* thread_autobus(void* args) {
	while (1) {
		// esperar a que los viajeros suban y bajen
		Autobus_En_Parada();
		// conducir hasta siguiente parada
		Conducir_Hasta_Siguiente_Parada();
	}
}

void* thread_usuario(void* arg) {
	int id_usuario = *((int*) arg);
	int a, b;
	// obtener el id del usario
	while (1) {
		a=rand() % N_PARADAS;
		do{
			b=rand() % N_PARADAS;
		} while(a==b);
		printf("--- El usuario [%d] ha decidido hacer la ruta [%d] -> [%d].\n", id_usuario, a, b);
		Usuario(id_usuario,a,b);
	}
}

void Usuario(int id_usuario, int origen, int destino) {
	// Esperar a que el autobus esté en parada origen para subir
	Subir_Autobus(id_usuario, origen);
	// Bajarme en estación destino
	Bajar_Autobus(id_usuario, destino);
}

int main(int argc, char* argv[]) {
	// Opcional: obtener de los argumentos del programa la capacidad del
	// autobus, el numero de usuarios y el numero de paradas
	if(argc == 4){
		MAX_USUARIOS = atoi(argv[1]);
		USUARIOS = atoi(argv[2]);
		N_PARADAS = atoi(argv[3]);
	}
	else if(argc == 2 || argc == 3) exit(1);

	printf("COMIENZA LA RUTA DEL AUTOBUS...\n");

	esperando_parada = (int*) malloc(sizeof(int)*N_PARADAS);
	esperando_bajar = (int*) malloc(sizeof(int)*N_PARADAS);
	th_usuario = (pthread_t*) malloc(sizeof(pthread_t)*USUARIOS);
	int id[USUARIOS];
	
	// Definición de variables locales a main
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&en_parada, NULL);
	pthread_cond_init(&subir, NULL);
	pthread_cond_init(&bajar, NULL);

	// Crear el thread Autobus
	pthread_create(&th_bus, NULL, thread_autobus, NULL);

	for (int i = 0; i < USUARIOS; i++){
		id[i] = i;
		// Crear thread para el usuario i
		pthread_create(&th_usuario[i], NULL, thread_usuario, (void*)&id[i]);
	}

	//esperar hasta que acabe el autbus (en este codigo esta siempre circulando)
	pthread_join(th_bus, NULL);

	for(int i = 0; i < USUARIOS; i++){
		// Espear a que acaben los hilos de los usuarios i
		pthread_join(th_usuario[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&en_parada);
	pthread_cond_destroy(&subir);
	pthread_cond_destroy(&bajar);
	return 0;
}

void Autobus_En_Parada(){
/*Ajustar el estado y bloquear al autobús hasta que no haya pasajeros que
quieran bajar y/o subir la parada actual. Después se pone en marcha*/
	pthread_mutex_lock(&mutex);
	estado = EN_PARADA;
	printf("Autobus en parada [%d].\n", parada_actual);
	while(esperando_parada[parada_actual] > 0 || esperando_bajar[parada_actual] > 0){
		pthread_cond_broadcast(&en_parada);
		if(esperando_bajar[parada_actual] > 0){
			pthread_cond_wait(&bajar, &mutex);
		}
		if(esperando_parada[parada_actual] > 0){
			pthread_cond_wait(&subir, &mutex);
		}
	}
	pthread_mutex_unlock(&mutex);
}

void Conducir_Hasta_Siguiente_Parada(){
/*Establecer un Retardo que simule el trayecto y actualizar numero de parada*/
	pthread_mutex_lock(&mutex);
	estado = EN_RUTA; //COMIENZA A IR A LA SIGUIENTE PARADA
	printf("----------  Autobus en ruta  ----------\n");
	pthread_mutex_unlock(&mutex);

	sleep(rand()%10); //DORMIMOS PARA SIMULAR EL TRAYECTO

	pthread_mutex_lock(&mutex);
	parada_actual = ++parada_actual % N_PARADAS;
	estado = EN_PARADA;
	printf("Autobus acaba de llegar a la parada [%d].\n", parada_actual);
	pthread_mutex_unlock(&mutex);
}

void Subir_Autobus(int id_usuario, int origen){
/*	El usuario indicará que quiere subir en la parada ’origen’, esperará a que
el autobús se pare en dicha parada y subirá. El id_usuario puede utilizarse para
proporcionar información de depuración*/
	pthread_mutex_lock(&mutex);
	esperando_parada[origen]++;
	printf("Usuario [%d] esperando en la parada [%d] para SUBIR.\n", id_usuario, origen);

	while(parada_actual != origen || estado != EN_PARADA){
		pthread_cond_wait(&en_parada, &mutex);
	}

	n_ocupantes++;
	esperando_parada[origen]--;
	printf("Usuario [%d] en la parada [%d] acaba de SUBIR al autobus.\n", id_usuario, origen);
	if(esperando_parada[origen] == 0) pthread_cond_signal(&subir);
	pthread_mutex_unlock(&mutex);
}

void Bajar_Autobus(int id_usuario, int destino){
/*
El usuario indicará que quiere bajar en la parada ’destino’, esperará a que
el autobús se pare en dicha parada y bajará. El id_usuario puede utilizarse para
proporcionar información de depuración*/
	pthread_mutex_lock(&mutex);
	esperando_bajar[destino]++;
	printf("Usuario [%d] esperando para BAJAR en la parada [%d].\n", id_usuario, destino);

	while(parada_actual != destino || estado != EN_PARADA){
		pthread_cond_wait(&en_parada, &mutex);
	}

	n_ocupantes--;
	esperando_bajar[destino]--;
	printf("Usuario [%d] en la parada [%d] acaba de BAJAR del autobus.\n", id_usuario, destino);
	if(esperando_bajar[destino] == 0) pthread_cond_signal(&bajar);
	pthread_mutex_unlock(&mutex);
}