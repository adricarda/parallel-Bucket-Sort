#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct bucket {
	int n_elem;
	int index; // [start : n_elem)
	int start; //starting point in B array
};

int cmpfunc (const void * a, const void * b)
{
 return ( *(int*)a - *(int*)b );
}

int main(int argc, char *argv[]) {

	int *A, *B, *temp;
	int dim, n_buckets, i, w, j, limit;
	struct bucket* buckets; //array of buckets
	double t1; // Timing variables
	float total; //total time

	printf("Give length of array to sort \n");
	if (scanf("%d", &dim) != 1){
		printf("error\n");
		return -1;
	}
	printf("Give number of buckets \n");
	if (scanf("%d", &n_buckets) != 1){
		printf("error\n");
		return -1;
	}
	limit = 100000;
	w = (int)limit/n_buckets;
	A = (int *) malloc(sizeof(int)*dim);
	B = (int *) malloc(sizeof(int)*dim);
	
	buckets = (struct bucket *) calloc(n_buckets, sizeof(struct bucket));

	for(i=0;i<dim;i++) {
		A[i] = random() % limit;
	}
	
	if (dim <= 20) {
		printf("Unsorted data \n");
		for(i=0;i<dim;i++) {
			printf("%d ",A[i]);
		}
		printf("\n");
  	}

// ****************************
// Starting the main algorithm
// ****************************

	t1 = omp_get_wtime();
	
	for (i=0; i<dim;i++){
		j = A[i]/w;
		if (j>n_buckets-1)
				j = n_buckets-1;
		buckets[j].n_elem++;
	}
	
	//buckets[0].index=0; //bucket 0 starts from 0 in B, bucket 1 starts from the start of bucket 0 + the number of elements in bucket 0 ...
	//buckets[0].start=0;
	for (i=1; i<n_buckets;i++){
		buckets[i].index = buckets[i-1].index + buckets[i-1].n_elem;
		buckets[i].start = buckets[i-1].start + buckets[i-1].n_elem;
	}
	int b_index;
	for (i=0; i<dim;i++){
		j = A[i]/w;
		if (j > n_buckets -1)
				j = n_buckets-1;
		b_index = buckets[j].index++;
		B[b_index] = A[i];
	}	
	
	for(i=0; i<n_buckets; i++)
		qsort(B+buckets[i].start, buckets[i].n_elem, sizeof(int), cmpfunc);
		
	temp = A;
	A = B;
	B = temp;
	total = omp_get_wtime() - t1;
		
	if (dim <= 20) {
		printf("Sorted data \n");
		for(i=0;i<dim;i++) {
			printf("%d ",A[i]);
		}
		printf("\n");
	}

	printf("Sorting %d elements took %f seconds\n", dim,total);

	int sorted = 1;
	for(i=0;i<dim-1;i++) {
		if (A[i] > A[i+1])
			sorted = 0;
  	}
  	if (!sorted)
		printf("The data is not sorted!!!\n");
}
