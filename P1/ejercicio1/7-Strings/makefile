CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : strings1.o strings2.o strings1.c strings2.c 	
	gcc $(CFLAGS) -o S1 strings1.o $(LIBS)
	gcc $(CFLAGS) -o S2 strings2.o $(LIBS)


.PHONY: clean

clean:
	-rm *.o S1 S2 
