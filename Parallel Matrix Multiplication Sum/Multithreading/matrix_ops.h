#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

/* Function makeMatrix
*		@param int rows
*			- represents the row dimension of the matrix to be created
*		@param int columms 
*			- represents the column dimension of the matrix to be created
*		@return int **matrix
*			- represents the created matrix
*/
int** makeMatrix(int rows, int columns);


/* Function destroyMatrix
*		@param int **matrix
*			- represents the matrix to be freed
*		@param int row 
*			- represents the rows to be freed
*/
void destroyMatrix(int **matrix, int rows);


/* Function matrixMultiply
*		@param int **ptrA, **ptrB, **ptrC
*			- represents each of the matrices 
*		@param int row 
*			- represents the current row to be calculated
*		@param int M, N, K 
*			- represents the dimensions of the matrices
*		@return int **ptrC
*			- updates ptrC for the invoking thread
*/
int **matrixMultiply(int **ptrA, int **ptrB, int **ptrC, int row, int M, int N, int K);
#endif