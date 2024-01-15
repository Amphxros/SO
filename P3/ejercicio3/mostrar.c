#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>

void mostrar_contenido(char* fichero, off_t offset){
	int fd= open(fichero, O_RDONLY);
	if(fd==-1){
		perror("Error al abrir el fichero");
		exit(EXIT_FAILURE);
	}

	// Situar el marcador de posición
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("Error al posicionar el marcador de posición");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char buffer;
    ssize_t bytes_leidos;

    // Leer byte a byte hasta el final del fichero
    while ((bytes_leidos = read(fd, &buffer, 1)) > 0) {
        write(STDOUT_FILENO, &buffer, 1);
    }

    if (bytes_leidos == -1) {
        perror("Error al leer el fichero");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int main(int argc, char *argv[])
{
	int opt;
	int flagN=0; // - n
	int flagE=0; // -e
	off_t offset=0; //N value

	while((opt = getopt(argc, argv, "n:e")) !=-1){
		switch(opt){
			case 'n':
				flagN=1;
				offset= atoi(optarg);
			break;

			case 'e':
				flagE=1;
			break;

			default:
			  fprintf(stderr, "Uso: %s [-n N] [-e] fichero\n", argv[0]);
			break;
		}
	}

	 // Verificar que se proporcionó el nombre del fichero
    if (optind >= argc) {
        fprintf(stderr, "Se espera el nombre del fichero como argumento\n");
        exit(EXIT_FAILURE);
    }

	if(flagE){
		struct stat stat_buffer;
		if(stat(argv[optind], &stat_buffer)==-1){

		}
		offset=stat_buffer.st_size - offset;

	}

	

	return 0;
}