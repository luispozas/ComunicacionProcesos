#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

//MINGYANG CHEN X9805942F 
//LUIS POZAS PALOMO

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes)
{
	int nb = 0;
	int c;

	if(origin == NULL || destination == NULL) return -1;

	do {
		c = getc(origin);
		if (c == EOF) break;
		putc((unsigned char)c, destination);
		nb++;
	} while (nb < nBytes && c != EOF);

	return nb;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file)
{
	int c, tam=0;
	char *str;

	do{
		c=getc(file);
		tam++;
	}while((c!=(int)'\0')&& c!=EOF);

	if((str=(char*)malloc(tam)) == NULL || c == EOF) return NULL;

	fseek(file,-tam,SEEK_CUR);
	fread(str,1,tam,file);
	return str;

}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles)
{
	int i;
	stHeaderEntry *header;

	//Leemos el número de ficheros (N) del tarFile y lo copiamos en nFiles.
	fread(nFiles, sizeof(int), 1, tarFile);

	header = (stHeaderEntry *) malloc(sizeof (stHeaderEntry) * (*nFiles));

	for (i = 0; i < *nFiles; i++) {
		header[i].name = loadstr(tarFile); //Usamos loadstr para cargar el nombre en header[i].name
		fread(&header[i].size, sizeof(int), 1, tarFile); //Leemos el tamaño del fichero y lo almacenamos en header[i].size
	}
	return header;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[])
{
	 // Completar la función
	   FILE *inputFile = NULL;
	   FILE *tarFile = NULL;

	   int nBytes = sizeof(int);

	   if((tarFile = fopen(tarName, "w")) == NULL) return (EXIT_FAILURE);

	   stHeaderEntry *header = malloc(sizeof(stHeaderEntry) * nFiles);

	   //Se copia la cabecera
	   for(int i = 0; i < nFiles; i++) {
	      header[i].name = malloc(strlen(fileNames[i]) + 1);
	      strcpy(header[i].name,fileNames[i]);
	      nBytes += sizeof(header->size) + strlen(fileNames[i]) + 1;
	   }

	   // Desplazamos el indicador toda la cabecera
	   fseek(tarFile, nBytes, SEEK_SET);

	   //Copiamos el contenido de los ficheros
	   for(int i = 0; i < nFiles; i++) {
	      inputFile = fopen(fileNames[i], "r");
	      header[i].size =  copynFile(inputFile, tarFile, INT_MAX);
	      fclose(inputFile);
	   }

	   // Nos ponemos en el byte 0 del fichero tar
	   rewind(tarFile);

	   //Escribimos el numero de ficheros
	   fwrite(&nFiles, sizeof(int), 1, tarFile);

	   //escribir la ruta del fichero y lo que ocupa
	   for(int i = 0; i < nFiles;i++) {
	      fwrite(header[i].name, 1, strlen(header[i].name)+1, tarFile);	      
	      fwrite(&header[i].size, sizeof(unsigned int), 1, tarFile);	 
	      free(header[i].name);
	   }

	   printf("CORRECTO, creado fichero mtar\n");

	   free(header);
	   fclose(tarFile);

	   return (EXIT_SUCCESS);
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathnamels
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[]){

	// Completar la función
	FILE *outputFile;
	FILE *tarFile;

	int numF = 0;
	if((tarFile = fopen(tarName, "r")) == NULL) return (EXIT_FAILURE);

	//leemos la cabecera
	stHeaderEntry *header = readHeader(tarFile, &numF);

	//Se descomprime
	for(int i = 0; i < numF; i++) {
	  outputFile = fopen(header[i].name, "w");
	  copynFile(tarFile, outputFile, header[i].size);
	  printf("[%d]: fichero creado %s, tamano %d Bytes... \n", i, header[i].name, header[i].size);
	  fclose(outputFile);
	}

	fclose(tarFile);
	return (EXIT_SUCCESS);
}