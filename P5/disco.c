#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

// Definir semáforos y variables globales
int aforo_actual = 0;
int numVipsWaiting=0;



	
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //main process
pthread_cond_t vip_cond = PTHREAD_COND_INITIALIZER; // vip "bool"
pthread_cond_t normal_cond = PTHREAD_COND_INITIALIZER; // not vip "bool"

/*Hilo que espera mientras se cumpla una condici´on:

lock(mutex);
while (<conditional expresion>)
    wait(cond_var, mutex);

<acciones restantes en secci´on cr´ıtica>
unlock(mutex);

<acciones deseadas fuera de secci´on cr´ıtica>
*/

/*IMPORTANTE
wait: siempre en un bucle while
wait: siempre con el mutex cogido
signal: mejor con el mutex cogido*/

void enter_normal_client(int id)
{
	 pthread_mutex_lock(&mutex);

    while (aforo_actual >= CAPACITY && numVipsWaiting==0) //siempre que no hayan vips esperando y haya aforo
    {
        printf("Client %2d waiting to enter (normal)\n", id);
        pthread_cond_wait(&normal_cond, &mutex);
    }
	aforo_actual++;
    printf("Client %2d entered (normal). Aforo: %d\n", id, aforo_actual);

    pthread_mutex_unlock(&mutex);

}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&mutex);
	
    while (aforo_actual >= CAPACITY) //son vips, solo hace falta que haya aforo
    {
        printf("Client %2d waiting to enter (vip)\n", id);
        pthread_cond_wait(&vip_cond, &mutex);
    }

    aforo_actual++;
	numVipsWaiting--;
    printf("Client %2d entered (vip). Aforo: %d\n", id, aforo_actual);

    pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco ~\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

/*Se˜nalizaci´on desde otro hilo:
lock(mutex);

<operaciones que afectan a la expresi´on condicional>
signal(cond_var);

<otras operaciones protegidas>
unlock(mutex);*/

void disco_exit(int id, int isvip)
{
	 pthread_mutex_lock(&mutex);
	 aforo_actual--;
	 printf("Client %2d (%s) exited. Aforo: %d\n", id, VIPSTR(isvip), aforo_actual);
    // Liberar espacio para clientes que están esperando
    if (aforo_actual < CAPACITY)
    {
        if (pthread_cond_signal(&vip_cond) != 0)
        {
            perror("Error signaling VIP condition");
            exit(EXIT_FAILURE);
        }
        else if (pthread_cond_signal(&normal_cond) != 0)
        {
            perror("Error signaling normal condition");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_unlock(&mutex);

}

void *client(void *arg)
{
	int id = *((int *)arg);
    int isvip = *((int *)(arg + sizeof(int)));

	if(isvip){
		numVipsWaiting++;
    	enter_vip_client(id);	
	}
	else
		enter_normal_client(id);

	dance(id,isvip);
	disco_exit(id,isvip);

	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc != 2) //tiene que tener un argumento: nº de clientes
    {
        fprintf(stderr, "Usage: %s <number_of_clients>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_clients = atoi(argv[1]); 
    pthread_t clients[num_clients];// all the clients sin importar si son vips o no
    int client_data[num_clients][2]; // 0 not vip 1 vip, could be 2 arrays but everyone could be vip or not so at least all the clients * 2

    srand((int)time(NULL));

    // Inicializar semáforos y variables globales
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&vip_cond, NULL);
    pthread_cond_init(&normal_cond, NULL);

	//creamos los clientes 
    for (int i = 0; i < num_clients; i++)
    {
        client_data[i][0] = i + 1; // id
        client_data[i][1] = rand() % 2; // 0 for not vip, 1 for vip
        pthread_create(&clients[i], NULL, client, (void *)client_data[i]); // creacion del hilo 
		
	
    }

	printf("\n");
    for (int i = 0; i < num_clients; i++)
        pthread_join(clients[i], NULL);

    // Destruimos el proceso principal 
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&vip_cond);
    pthread_cond_destroy(&normal_cond);

	return 0;
}