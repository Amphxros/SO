#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

// Definir semáforos y variables globales
int aforo_actual = 0;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t vip_cond = PTHREAD_COND_INITIALIZER;
	pthread_cond_t normal_cond = PTHREAD_COND_INITIALIZER;
void enter_normal_client(int id)
{
	 pthread_mutex_lock(&mutex);

    while (aforo_actual >= CAPACITY)
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

    while (aforo_actual >= CAPACITY)
    {
        printf("Client %2d waiting to enter (vip)\n", id);
        pthread_cond_wait(&vip_cond, &mutex);
    }

    aforo_actual++;
    printf("Client %2d entered (vip). Aforo: %d\n", id, aforo_actual);

    pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

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
		enter_vip_client(id);
	}
	else{
		enter_normal_client(id);
	}

	dance(id,isvip);

	disco_exit(id,isvip);

	return NULL;

}

int main(int argc, char *argv[])
{
	if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number_of_clients>\n", argv[0]);
        exit(EXIT_FAILURE);
    }



    int num_clients = atoi(argv[1]);
    pthread_t clients[num_clients];
    int client_data[num_clients][2];

    srand((unsigned int)time(NULL));

    // Inicializar semáforos y variables globales
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&vip_cond, NULL);
    pthread_cond_init(&normal_cond, NULL);

    for (int i = 0; i < num_clients; i++)
    {
        client_data[i][0] = i + 1; // id
        client_data[i][1] = rand() % 2; // 0 for not vip, 1 for vip
        pthread_create(&clients[i], NULL, client, (void *)client_data[i]);
		printf("\n");
    
    }

    for (int i = 0; i < num_clients; i++)
    {
        pthread_join(clients[i], NULL);
    }

    // Destruir semáforos y mutex
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&vip_cond);
    pthread_cond_destroy(&normal_cond);



	return 0;
}
