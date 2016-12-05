/** pmms.c
*
* Author: Jad Sapida
* About: C file that contains the:
*			- main method
*			- method to create threads
*			- consumer method
*			- thread function
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "readfile.h"
#include "matrix_ops.h"
#include "pmms.h"

/* Shared thread data */
int **matrixA;
int **matrixB;
int **matrixC;
int subtotal[2];
int row = 0;
int M;
int N;
int K;

/* Thread mutex*/
pthread_mutex_t mutex;

/* Thread condition */
pthread_cond_t empty;
pthread_cond_t full;

int main(int argc, char *argv[]) {

	if(argc != 6) {
		printf("/***************************************/\n");
		printf("ERROR: Program needs exactly 6 parameters to run.\n");
		printf("Run as \"./pmms file_1 file_2 M N K\"\n");
		printf("/***************************************/\n");
	} else {
		/* Convert arguments to integers */
		M = atoi(argv[3]);
		N = atoi(argv[4]);
		K = atoi(argv[5]);

		/* Threads */
		pthread_t threads[M];

		/* Contains the total value */
		int total = 0;

		/* Initialise shared variables*/
		matrixA = makeMatrix(M, N);
	 	matrixB = makeMatrix(N, K);
		matrixC = makeMatrix(M, K);

        /* Write file contents to matrices */
		readFile(argv[1], matrixA, M, N);
		readFile(argv[2], matrixB, N, K);

		/* Initialise the first element of subtotal to 0 */
		subtotal[0] = 0;

		/* Initialise mutexes & conditions */
		pthread_mutex_init(&mutex, NULL);
		pthread_cond_init(&empty, NULL);
		pthread_cond_init(&full, NULL);

		/* Create M number of threads */
		createMThreads(M, threads);
		
		do {
			consumerThread(&total);

			/* If the last row has been reached exit the loop */
			if(row == M) {
					
				printf("------------------------------------------\n");
				printf("Final Total = %d\n", total);
				printf("------------------------------------------\n");
				break;
			}
		} while(1);
		
		/* Wait for created threads to exit */
		int ii;
		for(ii = 0; ii < M; ii++) {
			pthread_join(threads[ii], NULL);
		}
	}
	/* Destroy matrices */
	destroyMatrix(matrixA, M);
	destroyMatrix(matrixB, N);
	destroyMatrix(matrixC, M);

	/* Destroy mutexes and conditions */
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&empty);
	pthread_cond_destroy(&full);

	pthread_exit(NULL);
}

void createMThreads(int m, pthread_t *threads) {
	int ii;

	for(ii = 0; ii < m; ii++) {
		pthread_create(&threads[ii], NULL, producerThread, NULL);
	}
}

void consumerThread(int *total) {
	pthread_mutex_lock(&mutex);

	/* Wait until the producer produces a subtotal */
	while(subtotal[0] == 0) {
		pthread_cond_wait(&full , &mutex);
	}

	printf("------------------------------------------\n");
	printf("Parent Thread (%d)\n", pthread_self());
	printf("Current Total = %d\n", *total);
	printf("Adding Subtotal (%d) from Thread (%d)\n", subtotal[0], subtotal[1]);

	/* Add the subtotal to the current total */
	/* Consume the subtotal */
	*total += subtotal[0];
	printf("New Total = %d\n", *total);
	printf("------------------------------------------\n");

	/* Set subtotal to 0 for the next producer */
	subtotal[0] = 0;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&empty);
} 

void *producerThread(void *param) {
	pthread_mutex_lock(&mutex);

	/* Wait until the subtotal is consumed */
	while(subtotal[0] != 0) {
		pthread_cond_wait(&empty, &mutex);
	}

	printf("\n");
	printf("------------------------------------------\n");
	printf("Thread (%d) going in critical section\n", pthread_self());

	/* Multiply matrices A and B and update one row of matrixC */
	matrixC = matrixMultiply(matrixA, matrixB, matrixC, row, M, N, K);

	int temp = 0, ii;

	/* Use a temp variable so consumer waits for the producer */
	for(ii = 0; ii < K; ii++) {
		temp += matrixC[row][ii];
	}

	/* Increment row to access the next row of matrixC */
	row += 1;

	/* Produce the subtotal */
	subtotal[0] = temp;		

	/* Set 2nd element of subtotal to the pid of the invoking process */
	subtotal[1] = pthread_self();

	printf("Subtotal = %d\n", subtotal[0]);
	printf("------------------------------------------\n");
	printf("\n");

	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&full);

	pthread_exit(NULL);
}