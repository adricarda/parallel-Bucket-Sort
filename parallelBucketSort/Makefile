CC=gcc
CFLAGS= -g -O3
DEPS =
OBJ = parallelBucketSort.o
LIB = -fopenmp

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIB)

parallelBucketSort: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o parallelBucketSort
