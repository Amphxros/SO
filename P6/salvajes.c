#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define NUMITER 3
#define KEY_SEM "/caldero_sem"
#define KEY_SHM 1234

int sem_id, shm_id;

struct SharedMemory {
    int servings;
};

struct sembuf sem_wait = {0, -1, 0}; // Esperar
struct sembuf sem_signal = {0, 1, 0}; // Señal

struct SharedMemory *shared_mem;

/*void Consumer() {
...
mutex_lock(&mutex)
while( !num_elem )
    cond_wait(&event, &mutex);
element = extract();
cond_broadcast(&event);
mutex_unlock(&mutex);
consume(element);
...
}
*/

int getServingsFromPot(void)
{
    if (semop(sem_id, &sem_wait, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    // Verificar si hay raciones en el caldero
    int servings = shared_mem->servings;

    if (semop(sem_id, &sem_signal, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }

    return servings;
}

void eat(void)
{
    unsigned long id = (unsigned long)getpid();
    printf("Savage %lu eating\n", id);
    sleep(rand() % 5);
}

void savages(void)
{
    for (int i = 0; i < NUMITER; i++) {
        int servings = getServingsFromPot();
        if (servings > 0) {
            eat();
        } else {
            printf("Savage [%d] finds the pot empty\n", getpid());
        }
    }
}

int main(int argc, char *argv[])
{
    // Obtener semáforo
    sem_id = semget(ftok(KEY_SEM, 'R'), 1, 0);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Obtener memoria compartida
    shm_id = shmget(ftok(KEY_SHM, 'R'), sizeof(struct SharedMemory), 0);
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

    // Ciclo de los salvajes
    savages();

    return 0;
}