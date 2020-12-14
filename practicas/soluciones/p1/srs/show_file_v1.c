#include <stdio.h>
#include <stdlib.h>
#include <err.h>
//MINGYANG CHEN X9805942F 
//LUIS POZAS PALOMO

int main(int argc, char* argv[]) {

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}


	int bufferSize = 1;
	char *buffer = (char*)malloc(sizeof(char)*bufferSize);

	/* Open file */
	FILE* file = fopen(argv[1], "r");

	/* Read file byte by byte */
	while (fread(buffer, sizeof(char), bufferSize, file)>0) {
		fwrite(buffer, sizeof(char), bufferSize, stdout);
	}

	free(buffer);
	fclose(file);
	return 0;
}
