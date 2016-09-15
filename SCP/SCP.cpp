/**
 * Program will operate on single instances of the set covering problem
 * Arguments:
 *		1. string:			Filename representing the input file to deal with
 *		2. string:			Represents the operation (section of part 2 of the assignment) to perform
 *		3. int:optional:	The current best known solution for the given input file (only required
 *								only required if finding a solution and second argument is a search
 *								single-point or metaheurstic )

*/

#include "stdafx.h"
#include <stdlib.h>

/* Custom includes */
#include "matrix.h"
#include "problem_instance.h"

/* Constants */
#define FILE_OPEN_SUCCESS 0
#define NO_FILE_FOUND_ERROR_CODE "error_code:1:no_file_found"
#define INVALID_ARGUMENTS_ERROR_CODE "error_code:2:invalid_arguments"

#define TRUE 1
#define FALSE 0

typedef int boolean;

/* Function prototypes */

boolean valid_arguments(int argument_count, char * arguments[]);
void generate_problem_instance(Instance * instance, FILE * file);

int main(int argument_count, char * argv[])
{
	if (!valid_arguments(argument_count, argv)) {
		return 0;
	}

	FILE * file;
	char * input_file_name = "test.txt";
	errno_t file_open_status = fopen_s(&file, input_file_name, "r");

	// Ensure that the input file opened correctly
	if (file_open_status != FILE_OPEN_SUCCESS) {
		printf("%s Opening input file %s failed", NO_FILE_FOUND_ERROR_CODE, input_file_name);
		return 0;
	}
	

	Instance instance;

	generate_problem_instance(&instance, file);

	print_instance(&instance);

	//print_matrix(instance.instance_matrix, instance.column_count, instance.row_count);
	
	return 0;
}



// Checks the provided arguments, at minimum there should be a filename
// and an operation mode
boolean valid_arguments(int argument_count, char * arguments[]) {
	//printf("%d %s", argument_count, arguments[0]);
	if (argument_count < 3) {
		// Default argument is the program name
		if (argument_count == 1) {
			printf("%s Missing arguments \n", INVALID_ARGUMENTS_ERROR_CODE);
			return 0;
		}
		printf("%s Missing required arguments.\n Found:", INVALID_ARGUMENTS_ERROR_CODE);
		for (int i = 0; i < argument_count; i++) {
			printf("%s", arguments[i]);
		}
		printf("\n");
		return 0;
	}
	return 1;
}

void generate_problem_instance(Instance * instance, FILE * file) {
	int row_count = 0, column_count = 0;
	int ** matrix;
	int * column_costs;
	
	// Read the rows and columns for the input and generate the basix matrix
	fscanf_s(file, "%d%d", &row_count, &column_count);
	matrix = generate_matrix(row_count, column_count);

	column_costs = (int *)calloc(column_count, sizeof(int));

	// Zero the matrix out so that we only need to flip things to '1'
	zero_matrix(matrix,  column_count, row_count);

	// Read in the column weights
	for (int i = 0; i < column_count; i++) {
		int cost;
		fscanf_s(file, "%d", &cost);
		column_costs[i] = cost;
	}

	// Now read coverings of rows for particular columns
	// The input files are indexing shit from 1, so we will have to compensate
	// e.g. 2 \n 3 4 says that row is covered by columsn 3 and 3, which map to 2 and 4
	// in the matrix
	int coverings = 0;
	int covering_column = 0;
	for (int row = 0; row < row_count; row++) {
		// First scan in the number of coverings this row has
		fscanf_s(file, "%d", &coverings);
		// Now iterate over the coverings, flipping the needed 'bits'
		for (int column = 0; column < coverings; column++) {
			fscanf_s(file, "%d", &covering_column);
			matrix[row][column - 1] = 1;
		}
	}

	instance->column_costs = column_costs;
	instance->column_count = column_count;
	instance->row_count = row_count;
	instance->instance_matrix = matrix;
}


