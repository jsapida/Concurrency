#ifndef READFILE_H
#define READFILE_H

/* Function readFile
*		@param char *fname
*			- represents the name of the file to be read
*		@param int *matrix
*			- represents the matrix to write on 
*		@param int rows, columns 
*			- represents the dimensions of the matrix
*/
void readFile(char *fname, int *matrix, int rows, int columns);
#endif