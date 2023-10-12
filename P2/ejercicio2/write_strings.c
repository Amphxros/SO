#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <nombre_archivo> <cadena1> <cadena2> ... <cadenaN>\n", argv[0]);
        exit(1);
    }

    FILE* file = fopen(argv[1], "w"); //w para permiso de escritura
    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(2);
    }

    for (int i = 2; i < argc; i++) {
        const char* str = argv[i];
        size_t len = strlen(str);

        // Escribe la cadena y su carácter terminador en el archivo
        fwrite(str, 1, len + 1, file);
    }
	
    fclose(file);
    return 0;
}
