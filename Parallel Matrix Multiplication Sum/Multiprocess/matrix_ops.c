/** matrix_ops.c
*
* Author: Jad Sapida
* About: C file that contains:
*			- A method that does the matrix multiplication per row
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix_ops.h"

int *matrixMultiply(int *ptrA, int *ptrB, int *ptrC, int row, int M, int N, int K) {
	int ii, jj, kk, sum = 0;

	/* Calculate which row to do */
	double ll = ceil((double)M/(M - row));

	for(ii = row; ii < ll; ii++) {
		for(jj = 0; jj < K; jj++) {
			for(kk = 0; kk < N; kk++) {
				/* sum += ptrA[ii][kk] * ptrB[kk][jj] */
				sum += ptrA[ii * N + kk] * ptrB[kk * K + jj];
			}
			/* Set ptrC[ii][jj] to sum then reset sum */
			ptrC[ii * K + jj] = sum;
			sum = 0;
		}
	}
	return ptrC;
}