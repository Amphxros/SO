#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*programa que temporiza la ejecución de un proceso hijo */
int main(int argc, char **argv)
{
	if(argc < 2){
		printf("-usage : ");
	}
	pid_t pid= fork();
	if(pid==-1){
		perror("thread");
	}
	else if(pid==0){
 		if (execvp(argv[1], &argv[1]) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
	}
	else{
		// Proceso padre
        // Temporizar la ejecución del proceso hijo (por ejemplo, 5 segundos)
        sleep(5);

        // Esperar a que finalice el hijo
        int status;
        pid_t child_pid = wait(&status);

        // Comprobar la causa de la finalización del hijo
        if (WIFEXITED(status)) {
            printf("Padre: El hijo (PID %d) ha finalizado normalmente con código %d.\n", child_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Padre: El hijo (PID %d) ha sido terminado por la señal %d.\n", child_pid, WTERMSIG(status));
        }

        exit(EXIT_SUCCESS);
	}

	return 0;
}