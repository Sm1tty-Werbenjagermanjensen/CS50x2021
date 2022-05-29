CC=gcc
DEPS = dictionary.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

speller: dictionary.o speller.o
	$(CC) -o speller dictionary.o speller.o
