#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

int aforo_actual = 0;
int numVipsWaiting = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t vip_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t normal_cond = PTHREAD_COND_INITIALIZER;

void enter_normal_client(int id) {
    pthread_mutex_lock(&mutex);
    while (aforo_actual >= CAPACITY && numVipsWaiting == 0) {
        printf("Client %2d waiting to enter (normal)\n", id);
        pthread_cond_wait(&normal_cond, &mutex);
    }
    aforo_actual++;
    printf("Client %2d entered (normal). Aforo: %d\n", id, aforo_actual);
    pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id) {
    pthread_mutex_lock(&mutex);
    while (aforo_actual >= CAPACITY) {
        printf("Client %2d waiting to enter (vip)\n", id);
        pthread_cond_wait(&vip_cond, &mutex);
    }
    aforo_actual++;
    numVipsWaiting--;
    printf("Client %2d entered (vip). Aforo: %d\n", id, aforo_actual);
    pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip) {
    printf("Client %2d (%s) dancing in disco ~\n", id, VIPSTR(isvip));
    sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip) {
    pthread_mutex_lock(&mutex);
    aforo_actual--;
    printf("Client %2d (%s) exited. Aforo: %d\n", id, VIPSTR(isvip), aforo_actual);
    if (aforo_actual < CAPACITY) {
        if (pthread_cond_signal(&vip_cond) != 0) {
            perror("Error signaling VIP condition");
            exit(EXIT_FAILURE);
        } else if (pthread_cond_signal(&normal_cond) != 0) {
            perror("Error signaling normal condition");
            exit(EXIT_FAILURE);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *client(void *arg) {
    int *client_data = (int *)arg;
    int id = client_data[0];
    int isvip = client_data[1];

    if (isvip) {
        numVipsWaiting++;
        enter_vip_client(id);
    } else {
        enter_normal_client(id);
    }

    dance(id, isvip);
    disco_exit(id, isvip);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int num_clients;
    fscanf(file, "%d", &num_clients);

    pthread_t *clients = (pthread_t *)malloc(num_clients * sizeof(pthread_t));
    if (clients == NULL) {
        perror("Error allocating memory for clients");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));

    for (int i = 0; i < num_clients; i++) {
        int isvip;
        fscanf(file, "%d", &isvip);
        int *client_data = (int *)malloc(2 * sizeof(int));
        if (client_data == NULL) {
            perror("Error allocating memory for client data");
            exit(EXIT_FAILURE);
        }
        client_data[0] = i; // Client ID
        client_data[1] = isvip; // Convert char to int

        pthread_create(&clients[i], NULL, client, (void *)client_data);
    }

    fclose(file);

    for (int i = 0; i < num_clients; i++)
        pthread_join(clients[i], NULL);

    free(clients);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&vip_cond);
    pthread_cond_destroy(&normal_cond);

    return 0;
}
