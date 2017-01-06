/***********************************************************************
* 	Matrix Multiplication: Using Threads for Parallelism				
*       @Author: Nefari0uss	
*       @License: MIT
*																
*	Programming solution for matrix multiplication				
*	that takes advantage of multiple threads					
*	executing in parallel.			
*							
*   Compile: gcc matrix-multiplcation.c -o mm -Wall -lpthread -lrt -O3		
*   Run: ./mm n where n is an int.							
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// dimensions:
#define N 2400
#define M 2000
#define P 500

// necessary to pass multiple parameters to multiply function
struct threadParams {
	int i, j, k, n, m, p;
};


// matrices
int **A;
int **B;
int **C;
int **C1;

// global variables
int threadCount;
int errors = 0;

// allocate space on the heap
void allocateSpace() {
	A = malloc(N * sizeof(int *));
	B = malloc(M * sizeof(int *));
	C = malloc(N * sizeof(int *));
	C1 = malloc(N * sizeof(int *));
	
	int i;
	for(i = 0; i < N; i++) {
		A[i] = malloc(N * sizeof(int));
		C[i] = malloc(N * sizeof(int));	
		C1[i] = malloc(N * sizeof(int));	
	}
	
	for(i = 0; i < M; i++) {
		B[i] = malloc(M * sizeof(int));
	}
}

void print20UniqueRowsCols(char c) {
	int i = 0, j = 0;
	
	if (c == 'A') {
		printf("Unique Rows and Columns from Matrix A:");
		for (i = 0 ; i <= 20 ; i++) {
			printf("[Element from Row: %d, Column: %d]: ", i, j);
			printf("%d\n", A[i][j]);
			j++;	
		}
	}
	
	if (c == 'B') {
		printf("Unique Rows and Columns from Matrix B:");
		for (i = 0 ; i <= 20 ; i++) {
			printf("[Element from Row: %d, Column: %d]: ", i, j);
			printf("%d\n", B[i][j]);
			j++;	
		}
	}
	
	if (c == 'C') {
		printf("Unique Rows and Columns from Matrix C:");
		for (i = 0 ; i <= 20 ; i++) {
			printf("[Element from Row: %d, Column: %d]: ", i, j);
			printf("%d\n", C[i][j]);
			j++;	
		}
	}
}


// print matrix
void printMatrix(char c) {
	int a, b, row, col;
	if (c == 'A') {
	
		row = N;
		col = M;
		
		for (a = 0; a < row; a++) {
			printf("[ ");
			for (b = 0; b < col; b++) { 
				printf("%d\t", A[a][b]);
			}
			printf("]\n");
		}
	} else if (c == 'B') {
		
		row = M;
		col = P;
		
		for (a = 0; a < row; a++) {
			printf("[ ");
			for (b = 0; b < col; b++) { 
				printf("%d\t", B[a][b]);
			}
			printf("]\n");
		}
		
	} else if (c == 'C') {
		
		row = N;
		col = P;
		
		for (a = 0; a < row; a++) {
			printf("[ ");
			for (b = 0; b < col; b++) { 
				printf("%d\t", C[a][b]);
			}
			printf("]\n");
		}
		
	} else if (c == 'D') {
		
		row = N;
		col = P;
		
		for (a = 0; a < row; a++) {
			printf("[ ");
			for (b = 0; b < col; b++) { 
				printf("%d\t", C1[a][b]);
			}
			printf("]\n");
		}
		
	}
}

// set up function for matrix A:
void generate_matrix_A() {
	printf("Generating A.\n");
	
	int i, j = 0;
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++ ) {
			A[i][j] = j - i + 2;	
		}
	}	

	//printMatrix('A');
	//print20UniqueRowsCols('A');
}

// set up function for matrix B:
void generate_matrix_B() {
	printf("Generating B.\n");
	int i, j = 0;
	
	for (i = 0; i < M; i++) {
		for (j = 0; j < P; j++ ) {
			B[i][j] = i - j + 1;	
		}
	}
	
	//printMatrix('B');
	//print20UniqueRowsCols('B');
	
}

// reset Matrix C:
void reset_matrix_C() {
	int i, j = 0;
	
	for (i = 0; i < N; i++) {
		for (j = 0; j < P; j++ ) {
			C[i][j] = 0;	
		}
	}
}

void storeC() {
	printf("\nStoring C.\n");
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < P; j++) {
			C1[i][j] = C[i][j];
		}
	}
}

int checkErrors() {
	printf("\nChecking for errors...");
	int i, j, error = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < P; j++) {
			if (C1[i][j] != C[i][j]) {
				error = 1;
			}
		}
	}
	
	if (error == 0) {
		printf("No errors.\n");
		return 0;
	} else {
		printf("Errors found.\n");
		return 1;
	}
}

// multiply matrix
void *multiply(void *args) {
	struct threadParams *variables = args;
		
	int i = variables->i;
	int j = variables->j;
	int k = variables->k;
	int n = variables->n;
	int m = variables->m;
	int p = variables->p;
	
	for (; i < n; i++) {
		for (j=0; j < p; j++) {
			C[i][j] = 0;
			for (k=0; k < m; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	
	return 0;
}

// multiply with the given number of threads.
double multiplyWithThreads(int threadsToUse) {
	
	double start, end;
	double cpu_time;
	
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	start = tv.tv_sec + (tv.tv_usec/1000000.0);
	
	printf("Multiplying with %d threads. \t", threadsToUse);
	pthread_t threads[threadsToUse];
	
	
	
	int z;
	for (z = 0; z < threadsToUse; z++) {
		//printf("\nz: %d\t", z);
		//printf("i: %d \tn: %d", args[z]->i, args[z]->n);
	
		struct threadParams *args = malloc(sizeof(struct threadParams));
		
		if (N % threadsToUse == 0) {
			args->i = N / threadsToUse * z;
			args->n = N / threadsToUse * (z + 1);
		} else {
			args->i = (N / threadsToUse + 1) * z;
			args->n = (N / threadsToUse + 1) * (z + 1);
		}
		args->m = M;
		args->p = P;
	
		if (args->n > N || (N / threadsToUse * (z + 2)) > N) {
			args->n = N;
		}
	
		pthread_attr_t pt_attr;
	
		// get default attributes
		pthread_attr_init(&pt_attr);
			
		// create thread 
		int create_thread_result = pthread_create(&threads[z], &pt_attr, multiply, args);
	
		// check for errors
		if (create_thread_result  == -1) {
			printf("Error creating producer thread.\n");
			return -1;
		}
	}
	
	// wait for threads to finish.
	for (z = 0; z < threadsToUse; z++) {
		pthread_join(threads[z], NULL);
	}
	
	gettimeofday(&tv, NULL);
	end = tv.tv_sec + (tv.tv_usec/1000000.0);
	
	if (threadsToUse == 1) {
		storeC();
		//printMatrix('D');	
	} else {
		if(checkErrors()) {
			errors++;
			//printMatrix('C');
		}	
	}
		
	cpu_time = ((double)(end - start));
	return cpu_time;
}

int main(int argc, char *argv[]) {

	if (argc !=2) {
		printf("Incorrect number of arguments.  Usage: ./mm.out <number-of-threads>\n");
		return 0;
	} else {
		threadCount = atoi(argv[1]);
	}

	allocateSpace();
	generate_matrix_A();
	generate_matrix_B();
	printf("\n");

	int i;
	for(i = 1; i <= threadCount; i++) {
		reset_matrix_C();
		double time = multiplyWithThreads(i);
		printf("Time for %d threads: %fs.\n\n", i, time);
	}
	
	printf("%d threads with errors encountered. \n", errors);
	
	return 0;
}
