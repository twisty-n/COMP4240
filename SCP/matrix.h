#pragma once

/**
 Generates an multidimensional array with the given column and row count accessed with x[i][j]
 i: row index, j: column index
 Inputs:
 			int: the number of rows the "matrix" should have
 			int: the number of columns the "matrix" should have
 
 Outputs:
			**int: Pointer to the array of pointers
*/
int ** generate_matrix(int, int);

/**
 Dealloactes a multidimensional array
 Inputs:
 			**int:	The pointer to the "matrix"
 			int:	THe number of rows 
*/
void free_matrix(int **, int);

/*
 Prints the contents of a matrix
 Inputs:
			int: the column count of the matrix
			int: the row count of the matrix
*/
void print_matrix(int **, int, int);

/*
 Zeros a given matrix
 Inputs:
			int: the column count of the matrix
			int: the row count of the matrix
*/
void zero_matrix(int **, int, int);