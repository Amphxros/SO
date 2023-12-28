#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "rb")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file using fread and write using fwrite */
    size_t buffer_size = 1024;  // You can adjust the buffer size
    unsigned char buffer[buffer_size];

    size_t bytesRead;

	while ((bytesRead = fread(buffer, 1, buffer_size, file)) > 0) {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, stdout);

        if (bytesWritten < bytesRead) {
            fclose(file);
            fprintf(stderr, "Error writing to stdout\n");
            exit(3);
        }
    }

    fclose(file);
    return 0;
}
