/*Luis Pozas Palomo 01946200-D, Ming yang Chen x9805942F*/

#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h> 

static const char secuencia[4]={'1','2','3','\0'};

#define IZQ 0 //Se mueve hacia la izquierda
#define DCHA 1 //Se mueve hacia la derecha
#define TIME 15

void mover(int leds, int type){
	int i=0;

	if(type == IZQ){
		for (i=0;i<TIME;i++){
			write(leds, &secuencia[i%3], sizeof(char));
			sleep(1);
		}
	}
	else{
		for (i=TIME-1;i>=0;i--){
			write(leds, &secuencia[i%3], sizeof(char));
			sleep(1);
		}
	}
}

int elegirSentido(){
	int op;
	printf("1.- Leds moviendose de izquierda a derecha.\n");
	printf("2.- Leds moviendose de derecha a izquierda.\n");
	printf("0.- SALIR\n");
	printf("Esperando opcion: ");
	scanf("%d", &op);
	return op;
}

int main() {
	int op;
	int leds = open("/dev/chardev_leds", O_RDWR);
	op = elegirSentido();
	while(op != 0){
		switch(op) {
			case 1: mover(leds, IZQ); break;
			case 2: mover(leds, DCHA); break;
			case 0: break;
			default: return 0;
		}
		op = elegirSentido();
	}
	close(leds);
	return 0;
}

