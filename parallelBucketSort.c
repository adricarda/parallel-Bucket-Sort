#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define max_threads 80

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
    int dim, n_buckets, i, w, limit, num_threads, workload, b_index;
    struct bucket* buckets; //array of buckets
    double t1; // Timing variable
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
	int *tmp;
    //global buckets
    int global_n_elem[n_buckets]; //number of elements in each bucket
    int global_starting_position[n_buckets]; //starting position in A for each bucket
    memset(global_n_elem, 0, sizeof(int)*n_buckets);
    memset(global_starting_position, 0, sizeof(int)*n_buckets);

    num_threads = n_buckets;
    omp_set_num_threads(num_threads);

    limit = 100000;
    w = limit/n_buckets;
    A = (int *) malloc(sizeof(int)*dim);
    B = (int *) malloc(sizeof(int)*dim);

    for(i=0;i<dim;i++) {
	    A[i] = random() % limit;
	}
	
	if (dim <= 40) {
        printf("Unsorted data \n");
        for(i=0;i<dim;i++) {
            printf("%d ",A[i]);
        }
        printf("\n");
    }

    //local buckets, n_buckets for each thread
    buckets = (struct bucket *) calloc(n_buckets*num_threads, sizeof(struct bucket));

// ****************************
// Starting the main algorithm
// ****************************

    t1 = omp_get_wtime();
#pragma omp parallel
{
    num_threads = omp_get_num_threads();

    int j,k;
    int local_index; // [0 : n_buckets)
    int real_bucket_index; // [0 : n_buckets * num_threads)
    int my_id = omp_get_thread_num();
    workload = dim/num_threads;
	int prevoius_index;

   #pragma omp for private(i,local_index)
    for (i=0; i< dim;i++){
        local_index = A[i]/w;
        if (local_index > n_buckets-1)
                local_index = n_buckets-1;
        real_bucket_index = local_index + my_id*n_buckets;
        buckets[real_bucket_index].n_elem++;
	}   
   
	int local_sum=0;
	for (j=my_id; j< n_buckets*num_threads; j=j+num_threads){
		local_sum += buckets[j].n_elem;
	}
	global_n_elem[my_id]=local_sum;
	
	#pragma omp barrier
	
	#pragma omp master
    {
	 for (j=1; j<n_buckets; j++){
		        global_starting_position[j] = global_starting_position[j-1] + global_n_elem[j-1];	 
				buckets[j].start = buckets[j-1].start + global_n_elem[j-1];
				buckets[j].index = buckets[j-1].index + global_n_elem[j-1];
		}
	}

	#pragma omp barrier
	for (j=my_id+n_buckets; j< n_buckets*num_threads; j=j+num_threads){
		int prevoius_index = j-n_buckets;
		buckets[j].start = buckets[prevoius_index].start + buckets[prevoius_index].n_elem;
		buckets[j].index = buckets[prevoius_index].index + buckets[prevoius_index].n_elem;	
	}
	#pragma omp barrier

    #pragma omp for private(i, b_index)
    for (i=0; i< dim ;i++){
        j = A[i]/w;
        if (j > n_buckets -1)
                j = n_buckets-1;
        k = j + my_id*n_buckets;
        b_index = buckets[k].index++;
        B[b_index] = A[i];
    }

#pragma omp for private(i)
    for(i=0; i<n_buckets; i++)
        qsort(B+global_starting_position[i], global_n_elem[i], sizeof(int), cmpfunc);
}
   total = omp_get_wtime() - t1;
	tmp = A;
	A = B;
	B = tmp;

   if (dim <= 40) {
        printf("A \n");
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


