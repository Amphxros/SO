#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define M 10
#define KEY_SEM "/caldero_sem"
#define KEY_SHM 1234

int finish = 0;
int sem_id, shm_id;

struct SharedMemory {
    int servings;
};

struct sembuf sem_wait = {0, -1, 0}; // Esperar
struct sembuf sem_signal = {0, 1, 0}; // Señal

struct SharedMemory *shared_mem;

/*void Producer() {
...
element = produce();
mutex_lock(&mutex)
while( num_elem == MAX_BUFFER)
    cond_wait(&event, &mutex);
insert(element);
cond_broadcast(&event);
mutex_unlock(&mutex);
...
}
*/

void putServingsInPot(int servings)
{
    if (semop(sem_id, &sem_wait, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    // Reponer raciones
    printf("Cocinero repone %d raciones en el caldero\n", servings);
    shared_mem->servings = servings;

    if (semop(sem_id, &sem_signal, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

void cook(void)
{
    while (!finish) {
        putServingsInPot(M);
    }
}

void handler(int signo)
{
    finish = 1;
}

int main(int argc, char *argv[])
{
    // Manejo de señales
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    // Crear semáforo
    sem_id = semget(ftok(KEY_SEM, 'R'), 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Inicializar semáforo
    semctl(sem_id, 0, SETVAL, 1);

    // Crear memoria compartida
    shm_id = shmget(ftok(KEY_SHM, 'R'), sizeof(struct SharedMemory), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Adjuntar memoria compartida
    shared_mem = (struct SharedMemory *)shmat(shm_id, NULL, 0);
    if (shared_mem == (struct SharedMemory *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Inicializar memoria compartida
    shared_mem->servings = 0;

    // Ciclo del cocinero
    cook();

    return 0;
}