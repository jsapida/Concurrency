#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

/* Function matrixMultiply
*		@param int *ptrA, *ptrB, *ptrC
*			- represents each of the matrices in the shared memory
*		@param int row 
*			- represents the current row to be calculated
*		@param int M, N, K 
*			- represents the dimensions of the matrices
*		@return int *ptrC
*			- updates ptrC for the invoking process
*/
int *matrixMultiply(int *ptrA, int *ptrB, int *ptrC, int row, int M, int N, int K);
#endif