#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct hilosDato
{
	int id;
	int priority;
};

void *thread_usuario(void *arg)
{
	struct hilosDato* datos= (struct hilosDato*) arg;
	
	printf("¡Hola desde el hilo! %d\n", datos->id);
    pthread_exit(NULL); //IMPORTANTE PARA TERMINAR EL PROCESO
}

int main(int argc, char* argv[])
{
	pthread_t id;
	struct hilosDato hilo;
	hilo.id = atoi(argv[1]);
	hilo.priority = atoi(argv[2]);
	if(pthread_create(&id, NULL, thread_usuario, &hilo)!=0){
		perror("thread");
		exit(1);
	}

	if(pthread_join(id, NULL)!=0){
		perror("joint");
		exit(1);
	}

 	printf("Programa principal\n");
	return 0;
}
