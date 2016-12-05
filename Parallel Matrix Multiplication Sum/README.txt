	Parallel Matrix Multiplication Sum

What is it?
-----------

The Parallel Matrix Multiplication Sum is a program that solves
the critical section problem using multiple processes/threads and
their inter-process/thread communications respectively.


How to compile
--------------

The solution comes with a Makefile for simpler compiling. Here 
are the steps to compile the program.

1. Make sure you are in the directory where all of the source 
code is located.

2. Type "make" in the command line.

Otherwise, if "make" is not available, refer to the Makefile for the libraries and compile order.

How to run
----------

1. Make sure you are in the directory where all of the source
code is located.

2. The program should be run as: 

	"./pmms matrixA matrixB M N K" 

where matrixA and matrix_B are the name of files that contain 
the entries of matrix A and matrix B respectively, and M, N, K 
specify the dimensions of the two matrices i.e. matrix A is a MxN matrix and matrix B is an NxK matrix.