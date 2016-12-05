/** pmms.c
*
* Author: Jad Sapida
* About: C file that contains the:
*			- main method
*			- method to create processes
*			- producer algorithm
*			- consumer method
*/
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include "readfile.h"
#include "matrix_ops.h"
#include "pmms.h"

int main(int argc, char *argv[]) {

	if(argc != 6) {
		printf("/***************************************/\n");
		printf("ERROR: Program needs exactly 6 parameters to run.\n");
		printf("Run as \"./pmms file_1 file_2 M N K\"\n");
		printf("/***************************************/\n");
	} else {
		/* Convert arguments to integers */
		int M = atoi(argv[3]);
		int N = atoi(argv[4]);
		int K = atoi(argv[5]);

		sem_t *empty, *full, *mutex;
		pid_t pid;

		/* Create the shared memory object */
		int shm_matrixA = shm_open("matA", O_CREAT | O_RDWR, 0600);
       	int shm_matrixB = shm_open("matB", O_CREAT | O_RDWR, 0600);
       	int shm_matrixC = shm_open("matC", O_CREAT | O_RDWR, 0600);
       	int shm_st = shm_open("st", O_CREAT | O_RDWR, 0600);
       	int shm_row = shm_open("row", O_CREAT | O_RDWR, 0600);

		/* Configure the size of the shared memory object */
        ftruncate(shm_matrixA, (M*N));
        ftruncate(shm_matrixB, (N*K));
        ftruncate(shm_matrixC, (M*K));
        ftruncate(shm_st, 2);
        ftruncate(shm_row, 1);

        /* Memory map the shared memory object */
        int *matrixA = (int *)mmap(NULL, (M*N), PROT_WRITE | PROT_READ, MAP_SHARED, shm_matrixA, 0);
        int *matrixB = (int *)mmap(NULL, (N*K), PROT_WRITE | PROT_READ, MAP_SHARED, shm_matrixB, 0);
        int *matrixC = (int *)mmap(NULL, (M*K), PROT_WRITE | PROT_READ, MAP_SHARED, shm_matrixC, 0);
        int *subtotal = (int *)mmap(NULL, 2, PROT_WRITE | PROT_READ, MAP_SHARED, shm_st, 0);
        int *row = (int *)mmap(NULL, 1, PROT_WRITE | PROT_READ, MAP_SHARED, shm_row, 0);

        /* Write to the shared memory object */
		readFile(argv[1], matrixA, M, N);
		readFile(argv[2], matrixB, N, K);

		/* Open semaphores */
		empty = sem_open("empty_sem", O_CREAT, 0600, 1);
		full = sem_open("full_sem", O_CREAT, 0600, 0);
		mutex = sem_open("mutex_sem", O_CREAT, 0600, 1);

		/* Initialise row to 0 so the first row of matrixC is done */
		*row = 0;
		subtotal[0] = 0;

		/* Create M number of processes */
        createMProcesses(M, &pid);

        /* Child process AKA producer */
		if(pid == 0) {
			sem_wait(empty);
			sem_wait(mutex);

			printf("\n");
			printf("------------------------------------------\n");
			printf("Child (%d) going in critical section\n", getpid());

			/* Multiply matrices A and B and update one row of matrixC */
			matrixC = matrixMultiply(matrixA, matrixB, matrixC, *row, M, N, K);

			int ii;
			/* Update the subtotal according to the row accessed */
			/* Produce the subtotal */
			for(ii = 0; ii < K; ii++) {
				subtotal[0] += matrixC[*(row) * K + ii];
			}

			/* Increment row to access the next row of matrixC */
			*row += 1;

			/* Set 2nd element of subtotal to the pid of the invoking process */		
			subtotal[1] = getpid();
			printf("Subtotal = %d\n", subtotal[0]);
			printf("------------------------------------------\n");
			printf("\n");

			sem_post(mutex);
			sem_post(full);

			/* Unmap the addresses */
			munmap(matrixA, (M*N));
			munmap(matrixB, (N*K));
			munmap(matrixC, (M*K));
			munmap(subtotal, 2);
			munmap(row, 1);

			/* Terminate the child process */
			exit(0);		
		} else {
			int total = 0;
			do {
				consumerProcess(full, mutex, empty, subtotal, &total);

				/* If the last row has been reached, unmap then exit the loop */
				if(*row == M) {
					/* Unmap the addresses */
					munmap(matrixA, (M*N));
					munmap(matrixB, (N*K));
					munmap(matrixC, (M*K));
					munmap(subtotal, 2);
					munmap(row, 1);
					
					printf("------------------------------------------\n");
					printf("Final Total = %d\n", total);
					printf("------------------------------------------\n");
					break;
				}
			} while(1);
		}
		/* Close file descriptors */
		close(shm_matrixA);
		close(shm_matrixB);
		close(shm_matrixC);
		close(shm_st);
		close(shm_row);

		/* Remove shared memory regions */
		shm_unlink("matA");
		shm_unlink("matB");
		shm_unlink("matC");
		shm_unlink("st");
		shm_unlink("row");

		/* Close semaphores */
		sem_close(empty);
		sem_close(full);
		sem_close(mutex);
	}
	return 0;
}

void createMProcesses(int m, pid_t *pid) {
	int ii;
	*pid = fork();

	for(ii = 1; ii < m; ii++) {
		/* Error happened */
		if(*pid < 0) {
			printf("ERROR: Could not fork() properly\n");
		/* Only parent forks*/
		} else if(*pid > 0) {
			*pid = fork();
		}
	}
}

void consumerProcess(sem_t *full, sem_t *mutex, sem_t *empty, int *subtotal, int *total) {
	sem_wait(full);
	sem_wait(mutex);

	printf("------------------------------------------\n");
	printf("Parent (%d)\n", getpid());
	printf("Current Total = %d\n", *total);
	printf("Adding Subtotal (%d) from Child (%d)\n", subtotal[0], subtotal[1]);

	/* Add the subtotal to the current total */
	/* Consume the subtotal */
	*total += subtotal[0];
	printf("New Total = %d\n", *total);
	printf("------------------------------------------\n");

	/* Set subtotal to 0 for the next producer */
	subtotal[0] = 0;
	sem_post(mutex);
	sem_post(empty);
} 