#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char *loadstr(FILE *file)
{
	char* buffer = NULL;
    size_t buffer_size = 0;

    while (1) {
        char c;
        if (fread(&c, 1, 1, file) != 1)
            break;

        if (c == '\0') {
            // Imprime la cadena y reinicia el búfer
            printf("%s\n", buffer);
            return buffer;
        } else {
            // Agrega el carácter al búfer
            buffer = (char*)realloc(buffer, buffer_size + 1);
            buffer[buffer_size] = c;
            buffer_size++;
        }
    }

    fclose(file);

	return buffer;	
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_archivo>\n", argv[0]);
        exit(1);
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error al abrir el archivo");
        exit(2);
    }

	char* buffer= loadstr(file);
    if (buffer != NULL) {
        free(buffer);
    }

    return 0;
}

