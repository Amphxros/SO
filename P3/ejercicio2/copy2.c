#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


#define BUFFER_SIZE 512

void copy(int fdo, int fdd)
{
	char buffer [BUFFER_SIZE];

	ssize_t read_, wrote_;

	while((read_ = read(fdo,buffer,BUFFER_SIZE)) > 0){
		wrote_= write(fdd,buffer,read_);
		if (wrote_ != read_) {
            perror("Error al escribir en el fichero destino");
            exit(EXIT_FAILURE);
        }
	}

	 if (read_ == -1) {
        perror("Error al leer el fichero origen");
        exit(EXIT_FAILURE);
    }
}

void copy_regular(char *orig, char *dest)
{
	int fd_origen= open(orig, O_RDONLY);
	if(fd_origen ==-1){
		perror("Error al abrir el fichero origen");
        exit(EXIT_FAILURE);
	}

	int fd_dest= open(dest, O_WRONLY | O_CREAT |O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd_dest ==-1){
		perror("Error al abrir/crear el fichero destino");
        close(fd_origen);
        exit(EXIT_FAILURE);
	}

	copy(fd_origen,fd_dest);
	close(fd_origen);
	close(fd_dest);
	printf("Copiado correcto de %s a %s\n", orig, dest);
}

void copy_link(char *orig, char *dest)
{
	struct stat stat_buffer;

	// Obtener información del enlace simbólico
    if (lstat(orig, &stat_buffer) == -1) {
        perror("Error al obtener información del enlace simbólico");
        exit(EXIT_FAILURE);
    }

	char* target_path= (char*) malloc(stat_buffer.st_size + 1);
	if(target_path == NULL){
		perror("Error al reservar memoria");
        exit(EXIT_FAILURE);
	}
	ssize_t target_size = readlink(orig, target_path, stat_buffer.st_size);
    if (target_size == -1) {
        perror("Error al leer la ruta apuntada por el enlace simbólico");
        free(target_path);
        exit(EXIT_FAILURE);
    }

    // Añadir el carácter null de final de cadena
    target_path[target_size] = '\0';

    // Crear un nuevo enlace simbólico que apunte a esta ruta
    if (symlink(target_path, dest) == -1) {
        perror("Error al crear el nuevo enlace simbólico");
        free(target_path);
        exit(EXIT_FAILURE);
    }

    free(target_path);

    printf("Copia del enlace simbólico %s a %s\n", orig, dest);
}

int main(int argc, char *argv[])
{
	if(argc !=3){
		fprintf(stderr, "Uso: %s <fichero_origen> <fichero_destino>\n", argv[0]);
        exit(EXIT_FAILURE);
	}

	struct stat stat_origen;
	// Obtener información del fichero origen
    if (lstat(argv[1], &stat_origen) == -1) {
        perror("Error al obtener información del fichero origen");
        exit(EXIT_FAILURE);
    }

    if (S_ISREG(stat_origen.st_mode)) {
        // Fichero regular
        copy_regular(argv[1], argv[2]);
    } else if (S_ISLNK(stat_origen.st_mode)) {
        // Enlace simbólico
        copy_link(argv[1], argv[2]);
    } else {
        // Otro tipo de fichero (por ejemplo, directorio)
        fprintf(stderr, "Error: El fichero origen no es un fichero regular ni un enlace simbólico.\n");
        exit(EXIT_FAILURE);
    }

	return 0;
}