CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : arg1.o arg2.o arg1.c arg2.c 	
	gcc $(CFLAGS) -o F1 arg1.o $(LIBS)
	gcc $(CFLAGS) -o F2 arg2.o $(LIBS)


.PHONY: clean

clean:
	-rm *.o F1 F2 
