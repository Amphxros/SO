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
	//critical reg
	int turn;
	pthread_mutex_lock(&mutex);
	turn = currentTicket++;
	while(numClients>=CAPACITY || vipWaiting==0){
		pthread_cond_wait(&cond, &mutex);
	}
	clientsInside++;
	currentTurn++;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
	

}

void enter_vip_client(int id)
{
	//critical reg
	int turn;
	pthread_mutex_lock(&mutex);
	turn = currentTicket++;
	currentVipTicket++;
	
	while(numClients>=CAPACITY){ //they are vip so they dont need to wait more
		pthread_cond_wait(&cond, &mutex);
	}
	clientsInside++;
	clientsVipInside++;
	currentTurn++;
	vipWaiting--;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);


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
		}
		else{
			enter_normal_client(c->id);
		}

		dance(c->id, c->isVip);
		
		free(c);
	}
	else{
		perror("Bad Format of client_arg_t");
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_fichero>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
	//open file
    FILE *file = fopen(argv[1], "r");
    //if file is null then exit bad
	if (!file) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
	pthread_mutex_init(&mutex,NULL);

    pthread_t* threads= malloc(MAX_CLIENTS* sizeof(pthread_t)); // all the client threads
	if(threads==NULL){
		perror("malloc bad");
		exit(EXIT_FAILURE);
	}
    int ids[MAX_CLIENTS]; //all the id of the clients (0,1,2,3)
    int vip_flags[MAX_CLIENTS]; //mark all the vip ones(1,0,0,1) 1=vip
	
	//create all the threads
	 for (int i = 0; i < MAX_CLIENTS; i++) {
		client_arg_t* c= malloc(sizeof(client_arg_t));
        c->id=i;
		
		if(fscanf(file, "%d", &c->isVip)=-1){
			perror("format of %d is incorrect", i);
			exit(EXIT_FAILURE);
		}

		vip_flags[i]= c->isVip;
		ids[i]=c->id;
        pthread_create(&threads[i], NULL, client, c);
        usleep(100); //Little pause between threads
    }
	
	


	return 0;
}
