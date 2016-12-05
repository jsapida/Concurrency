/** matrix_ops.c
*
* Author: Jad Sapida
* About: C file that contains:
*			- A method that creates a matrix
*			- A method that destroys a matrix
*			- A method that does the matrix multiplication per row
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix_ops.h"

int** makeMatrix(int rows, int columns) {
	int ii;
	int **matrix;
	
	matrix = (int**) malloc(rows * sizeof(int*));

	for(ii = 0; ii < rows; ii++) {
		matrix[ii] = (int*) malloc(columns * sizeof(int));
	}
	return matrix;
}

void destroyMatrix(int **matrix, int rows) {
	int ii;

	for(ii = 0; ii < rows; ii++) {
		free(matrix[ii]);
	}

	free(matrix);
}


int **matrixMultiply(int **ptrA, int **ptrB, int **ptrC, int row, int M, int N, int K) {
	int ii, jj, kk, sum = 0;

	/* Calculate which row to do */
	double ll = ceil((double)M/(M - row));

	for(ii = row; ii < ll; ii++) {
		for(jj = 0; jj < K; jj++) {
			for(kk = 0; kk < N; kk++) {
				sum += ptrA[ii][kk] * ptrB[kk][jj];
			}
			/* Set ptrC[ii][jj] to sum then reset sum */
			ptrC[ii][jj] = sum;
			sum = 0;
		}
	}
	return ptrC;
}