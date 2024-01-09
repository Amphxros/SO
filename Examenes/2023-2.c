#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NJUGADORES 4
#define MAXREBOTE 70
#define TOPEANOTACION 10 // Constante para el tope de anotación

enum estadoBalon_t { POSESION, ENAIRE };

struct jugador {
    int dorsal;
    int eficTiro;
    int eficRebote;
    int puntos;
};

enum estadoBalon_t balon;
int dificultadRebote = 0;
struct jugador jugadores[NJUGADORES] = {
    {0, 30, MAXREBOTE, 0},
    {1, 80, 20, 0},
    {2, 40, MAXREBOTE, 0},
    {3, 50, 50, 0}
};

pthread_mutex_t balonMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t balonCond = PTHREAD_COND_INITIALIZER;
int hilosActivos = NJUGADORES;
int finished = 0;

void tirar(struct jugador *jugador) {
    int dificultadTiro = rand() % 100;

    if (dificultadTiro < jugador->eficTiro) {
        // Canasta
        jugador->puntos += 2;
        printf("¡Canasta! Jugador %d suma 2 puntos. Puntos totales: %d\n", jugador->dorsal, jugador->puntos);

        if (jugador->puntos >= TOPEANOTACION) {
            // Indicar que el entrenamiento ha finalizado
            pthread_mutex_lock(&balonMutex);
            finished = 1;
            pthread_cond_broadcast(&balonCond);
            pthread_mutex_unlock(&balonMutex);
        } else {
            // Cambiar el estado del balón
            pthread_mutex_lock(&balonMutex);
            balon = ENAIRE;
            dificultadRebote = rand() % MAXREBOTE;
            pthread_cond_broadcast(&balonCond);
            pthread_mutex_unlock(&balonMutex);
        }
    } 
    else {
        printf("Tiro fallido. Jugador %d no suma puntos.\n", jugador->dorsal);
    }
}

void rebotear(struct jugador *jugador) {
    pthread_mutex_lock(&balonMutex);
    while (balon != ENAIRE || dificultadRebote >= jugador->eficRebote) {
        if (finished) {
            // Si el entrenamiento ha finalizado, salir del bucle
            pthread_mutex_unlock(&balonMutex);
            return;
        }
        pthread_cond_wait(&balonCond, &balonMutex);
    }

    // Conseguir el rebote
    balon = POSESION;
    printf("¡Rebote conseguido! Jugador %d tiene el balón.\n", jugador->dorsal);

    pthread_mutex_unlock(&balonMutex);
    // Realizar acciones adicionales después de conseguir el rebote (por ejemplo, correr())
}

void *jugadorInit(void *arg) {
    struct jugador *jugador = (struct jugador *)arg;

    while (1) {
        tirar(jugador); // Realizar un tiro

        // Si el entrenamiento ha finalizado, salir del bucle
        if (finished) break;

        usleep(100000); // Simular una pausa antes de rebotear
        rebotear(jugador); // Intentar conseguir un rebote
    }

    // Indicar que el hilo ha terminado
    pthread_mutex_lock(&balonMutex);
    hilosActivos--;
    pthread_mutex_unlock(&balonMutex);

    pthread_exit(NULL);
}

int main() {
    srand((unsigned int)time(NULL));

    pthread_t threads[NJUGADORES];

    for (int i = 0; i < NJUGADORES; i++) {
        if (pthread_create(&threads[i], NULL, jugadorInit, (void *)&jugadores[i]) != 0) {
            fprintf(stderr, "Error al crear el hilo para el jugador %d\n", i);
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    while (hilosActivos > 0) {
        usleep(1000);
    }

    // Mostrar los puntos de cada jugador al final
    for (int i = 0; i < NJUGADORES; i++) {
        printf("Jugador %d: Puntos totales = %d\n", jugadores[i].dorsal, jugadores[i].puntos);
    }

    return 0;
}
