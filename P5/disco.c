#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")
#include <pthread.h> 

struct{
	int id;
	int isVip;

}client_arg_t;

int currentTurn=0;
int currentVipTurn=0;
int currentTicket=0;
int currentVipTicket=0;
int vipWaiting=0;

int numClients=0;
int clientsInside=0;

pthread_mutex_t mutex;
pthread_cond_t cond;



void enter_normal_client(int id)
{

}

void enter_vip_client(int id)
{

}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{

}

void *client(void *arg)
{
	client_arg_t c= (client_arg_t) arg;

	if(c!=nullptr){
		printf("Hilo: %d y soy %d", c->id, c->isVip);

		if(c->isVip){
			enter_vip_client(c->id);
			vipWaiting--;
		}
		else{
			enter_normal_client(c->id);
		}
		currentTurn++;
		clientsInside++;

		dance(c->id, c->isVip);
		disco_exit(c->id, c->isVip);
	}
	else{
		perror("Bad Format");
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_fichero>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    int M;
    fscanf(file, "%d", &M);

    pthread_t threads[MAX_CLIENTS]; // all the client threads
    int ids[MAX_CLIENTS]; //all the clients could be normal clients
    int vip_flags[MAX_CLIENTS]; //or vip ones
	
	 for (int i = 0; i < M; i++) {
		client_arg_t c= client_arg_t();
		c->id=i;
        fscanf(file, "%d", &vip_flags[i]);
		c->isVip=&vip_flags[i];
        pthread_create(&threads[i], NULL, client, c);
        usleep(100); // Pequeña pausa entre la creación de hilos
    }
	
	


	return 0;
}
