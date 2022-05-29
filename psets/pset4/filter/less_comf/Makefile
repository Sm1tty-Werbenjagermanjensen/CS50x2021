CC=gcc
CFLAGS=-lm
LDFLAGS=-lm 
DEPS = helpers.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

filter: helpers.o filter.o
	$(CC) -o filter helpers.o filter.o $(CFLAGS)
