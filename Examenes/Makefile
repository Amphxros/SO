CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: 2023-2

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

2023-2: 2023-2.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all

clean:
	-rm 2023-2 2023-2.o
