CC=gcc
CFLAGS= -g -O3
DEPS =
OBJ = sequentialBucketSort.o
LIB = -fopenmp

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< $(LIB)

sequentialBucketSort: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o sequentialBucketSort	
