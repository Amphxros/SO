#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
       FILE* file = NULL;
    int c, ret;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    /* Abre el archivo */
    if ((file = fopen(argv[1], "r")) == NULL)
        err(2, "No se pudo abrir el archivo de entrada %s", argv[1]);

    /* Lee el archivo byte por byte */
    while ((c = getc(file)) != EOF) {
        /* Imprime el byte en stdout */
        ret = putc((unsigned char)c, stdout);

        if (ret == EOF) {
            fclose(file);
            err(3, "putc() failed");
        }
    }

    fclose(file);
    return 0;
}
