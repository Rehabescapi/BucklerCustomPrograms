CC= gcc
CFLAGS = -Wall -g

BINS = multitasking time


all: $(BINS)

%: %.c
		$(CC) $(CFLAGS) -o $@ $^ -pthread
time.o : 
	$(CC) -c time.c 

timemake: time.o 
	$(CC) -o time time.o  -pthread
	time ./time

clean :
	rm -rf $(BINS) *.o *.dSYM