CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : array1.o array2.o array1.c array2.c
	gcc $(CFLAGS) -o A1 array1.o $(LIBS)
	gcc $(CFLAGS) -o A2 array2.o $(LIBS)

.PHONY: clean

clean:
	-rm *.o A1 A2
