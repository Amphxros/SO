#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAMBLOQUE 512
void copy(char* fichero_origen, char* fichero_destino)
{
	printf("file\n");
	FILE* origin= open(fichero_origen, O_RDONLY);
	FILE* dest= open(fichero_destino, O_RDONLY);
	char* buffer = malloc(TAMBLOQUE* sizeof(char));
	
	
	if(read(origin, buffer,TAMBLOQUE)==-1){
		perror("Something went bad :( ");
		exit(1);
	}
	
	
	printf("all ok \n");
}

int main(int argc, char *argv[])
{
	printf("cpy\n");
	copy(argv[0], argv[1]);

	return 0;
}
