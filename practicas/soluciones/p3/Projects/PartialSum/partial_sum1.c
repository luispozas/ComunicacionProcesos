/*Luis Pozas Palomo 01946200-D, Ming yang Chen x9805942F*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t semaforo;
int total_sum = 0;

struct arg_struct {
	int ni;
	int nf;
};

void * partial_sum(void* arg) {
  struct arg_struct *args = arg;
  int ni = args->ni;
  int nf = args->nf;

  printf("-> ni: %d, nf: %d\n", ni, nf);

  for (int j = ni; j <= nf; j++){
  	sem_wait(&semaforo);
    total_sum = total_sum + j;
    sem_post(&semaforo);
  }

  pthread_exit(0);
}

int main(int argc, char* argv[]) {
  int num_hilos = atoi(argv[1]);
  int res = atoi(argv[2]);

  pthread_t hilos[num_hilos];

  //Dividimos las sumas segun los hilos a utilizar
  //para obtener los limites de las sumas parciales
  int nums[2*num_hilos];
  int cont = 1;
  int inc = (res/num_hilos);

  for(int i = 0; i < 2*num_hilos; i++){
  	nums[i] = cont;
  	i++;
  	if(i ==((2*num_hilos)-1)) nums[i] = res;
  	else{
  		cont += inc;
  		nums[i] = cont-1;
  	}
  }

  sem_init(&semaforo, 0, 1);

  //Creamos los hilos y les pasamos los argumentos (ni, nf)
  //que son los limites de las sumas parciales.
  struct arg_struct args[num_hilos];
  int idx = 0, arg[2];
  for(int i = 0; i < num_hilos; i++){
  	args[i].ni = nums[idx]; args[i].nf = nums[idx+1];
  	pthread_create(&hilos[i], NULL, partial_sum, (void*)&args[i]);
  	idx += 2;
  }

  //Esperamos a que terminen los hilos;
  for(int i = 0; i < num_hilos; i++){
  	pthread_join(hilos[i], NULL);
  }

  //Funcion para comprobar si el resultado es correcto.
  int myTot = 0;
  for(int i = 0; i <= res; i++){
  	myTot += i;
  }
  
  printf("total_sum=%d and it should be %d\n", total_sum, myTot);
  sem_destroy(&semaforo);
  return 0;
}
