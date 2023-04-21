CC= gcc
CFLAGS = -Wall -g

BINS = multitasking time


all: $(BINS)

%: %.c
		$(CC) $(CFLAGS) -o $@ $^ -pthread
time.o : 
	$(CC) -c time.c multitasking.c

timemake: time.o multitasking.o
	$(CC) -o time time.o multitasking.o -pthread
	time ./time

clean :
	rm -rf $(BINS) *.o *.dSYM