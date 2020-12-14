/*Luis Pozas Palomo 01946200-D, Ming yang Chen x9805942F*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_system(char* arg){
	int status;
	pid_t h = fork();
	if(h<0) printf("Error en el fork.\n");
	//El proceso hijo (mirar argumentos en man).
	else if(h == 0) execl("/bin/sh", "sh", "-c", arg, NULL); 
	else { //El proceso padre espera hasta que terminen los hijos.
		pid_t terminated_child = wait(&status);
		while( terminated_child != h)
			terminated_child = wait(&status);
	}
	return status;
}

int main(int argc, char* argv[]){
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}
	return my_system(argv[1]);
}

