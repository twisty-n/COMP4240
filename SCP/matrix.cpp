#include "stdafx.h"
#include "matrix.h"
#include <stdlib.h>

int ** generate_matrix(int row_count, int column_count) {
	// Declare the array that will hold all of the other arrays
	int ** _array = (int **)malloc(row_count * sizeof(*_array));

	// Check that the allocation succeeded, if it did, we are sweet, else fail with NULL
	if (!_array) {
		return NULL;
	} 

	for (int i = 0; i < row_count; i++) {
		_array[i] = (int *) malloc(sizeof *_array[i] * column_count);
	}

	return _array;
}

void free_matrix(int ** _array, int row_count) {
	// First dealloc indivudal rows
	for (int i = 0; i < row_count; i++) {
		free(_array[i]);
	}

	// Then free the top level
	free(_array);

}

void print_matrix(int ** matrix, int column_count, int row_count) {
	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < column_count; j++) {
			printf("%d", matrix[i][j]);
		}
		printf("\n");
	}
}

void zero_matrix(int ** matrix, int column_count, int row_count) {
	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < column_count; j++) {
			matrix[i][j] = 0;
		}
	}
}