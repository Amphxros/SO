#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAMBLOQUE 512
void copy(int fichero_origen, int fichero_destino)
{
	FILE* origin= fopen(fichero_origen,O_RDONLY); //fichero de lectura
	FILE* dest=fopen(fichero_destino, O_WRONLY); //fichero de escritura
	char* buffer=  malloc(TAMBLOQUE* sizeof(char)); //
	
	char c;
	int size=0;
	while(1){
		while(fread(&c,1,1,origin)==1 &&size<TAMBLOQUE){
			buffer[size]=c;
			printf("%s\n",buffer);
			size++;
		}

		if (size>=TAMBLOQUE)//si hemos llenado el bloque
		{
			for(int i=0;i<TAMBLOQUE;i++){
				const char* str = buffer[i];
				size_t len= strlen(str);
				fwrite(str,1,len+1,dest);
				buffer[i]=NULL;
			}
			size=0;
		}
		else{ //si no, hemos terminado de escribir
			const char* d;
			int i=0;
			do{
				d=buffer[i];
				i++;
				size_t len= strlen(d);
				fwrite(d,1,len+1,dest);
				buffer[i]=NULL;
			}while(d!=NULL);
			
			fclose(origin);
			fclose(dest);
			return;
		}
	}
}

int main(int argc, char *argv[])
{
	copy(argv[0], argv[1]);
	return 0;
}
