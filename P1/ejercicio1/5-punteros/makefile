CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : punteros1.o punteros2.o punteros3.o punteros1.c punteros2.c punteros3.c
	gcc $(CFLAGS) -o P1 punteros1.o $(LIBS)
	gcc $(CFLAGS) -o P2 punteros2.o $(LIBS)
	gcc $(CFLAGS) -o P3 punteros3.o $(LIBS)

.PHONY: clean

clean:
	-rm *.o P1 P2 P3
