#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>


#define BUFFER_SIZE 512
void copy(int fdo, int fdd)
{
	char buffer [BUFFER_SIZE];

	ssize_t read_, wrote_;

	while((read_ = read(fdo, buffer, BUFFER_SIZE)) > 0){
		wrote_ = write(fdd, buffer, read_);
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

int main(int argc, char *argv[])
{
	if(argc!=3){
		fprintf(stderr, "Uso: %s <fichero_origen> <fichero_destino>\n", argv[0]);
        exit(EXIT_FAILURE);
	}
	int fd_origen= open(argv[1], O_RDONLY);

	if(fd_origen==-1){
		perror("Error al abrir el fichero origen");
        close(fd_origen);
        exit(EXIT_FAILURE);
	}

	int fd_dest= open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	copy(fd_origen,fd_dest);
	close(fd_origen);
	close(fd_dest);

	return 0;
}