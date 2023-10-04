CC = gcc
CFLAGS = -Wall -g
LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : main2.o main2.c main1.o main1.c
	gcc $(CFLAGS) -o exe2 main2.o $(LIBS)
	gcc $(CFLAGS) -o exe1 main1.o $(LIBS)
.PHONY: clean

clean:
	-rm *.o exe2 exe1
