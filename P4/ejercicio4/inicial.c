#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void)
{
    int fd1, fd2, i;
    char c;
    char buffer[6];

    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR);
    write(fd1, "00000", 5);

    for (i = 1; i < 10; i++)
    {
        if (fork() == 0)
        {
            /* Child */
            sprintf(buffer, "%d", i * 11111);
            pread(fd1, NULL, 0, 0); // Seek to the end of the file atomically
            write(fd1, buffer, 5);
            close(fd1);
            exit(0);
        }
        else
        {
            /* Parent */
            sprintf(buffer, "%d", i * 11111);
            pread(fd1, NULL, 0, 0); // Seek to the end of the file atomically
            write(fd1, buffer, 5);
        }
    }

    // Wait for all children to finish
    while (wait(NULL) != -1);

    lseek(fd1, 0, SEEK_SET);
    printf("File contents are:\n");
    while (read(fd1, &c, 1) > 0)
        printf("%c", (char)c);
    printf("\n");
    close(fd1);
    exit(0);
}