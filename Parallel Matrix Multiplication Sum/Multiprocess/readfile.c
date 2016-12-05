/** readfile.c
*
* Author: Jad Sapida
* About: C file that contains:
*			- A method that reads the contents of a file and
*			  writes it in a matrix
*/
#include <stdio.h>
#include <stdlib.h>
#include "readfile.h"

void readFile(char *fname, int *matrix, int rows, int columns) {
	FILE* input;
	int ii, jj;

	input = fopen(fname, "r");

	/* Error checking */
	if(input == NULL) {
		perror("ERROR: an error occurred when opening the file");
	} else {
		for(ii = 0; ii < rows; ii++) {
			for(jj = 0; jj < columns; jj++) {
				/* &matrix[ii][jj] */
				fscanf(input, "%d", &matrix[ii * columns + jj]);
			}
		}
	}

	fclose(input);
}