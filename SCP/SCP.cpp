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
#include <string.h>

/* Custom includes */
#include "matrix.h"
#include "problem_instance.h"

/* Constants */
#define FILE_OPEN_SUCCESS 0
#define NO_FILE_FOUND_ERROR_CODE "error_code:1:no_file_found"
#define INVALID_ARGUMENTS_ERROR_CODE "error_code:2:invalid_arguments"

typedef int boolean;

/* Function prototypes */

boolean valid_arguments(int argument_count, char * arguments[]);
void generate_problem_instance(Instance * instance, FILE * file);
char * generate_output_file(char * input_file);

int main(int argument_count, char * argv[])
{
	if (!valid_arguments(argument_count, argv)) {
		return 0;
	}

	FILE * file;
	FILE * output_file;
	char * input_file_name = "scpnrh5.txt";

	char * output_file_path = generate_output_file(input_file_name);

	errno_t file_open_status = fopen_s(&file, input_file_name, "r");

	// Ensure that the input file opened correctly
	if (file_open_status != FILE_OPEN_SUCCESS) {
		printf("%s Opening input file %s failed", NO_FILE_FOUND_ERROR_CODE, input_file_name);
		return 0;
	}
	
	Instance instance;

	generate_problem_instance(&instance, file);
	fclose(file);

	// Open a file for output, write then close
	fopen_s(&output_file, output_file_path, "w");
	print_instance_to_file(&instance, output_file);
	fclose(output_file);

	// TODO: Free all the memory we allocated
	free_instance(&instance);
	free(output_file_path);

	return 0;
}

char * generate_output_file(char * input_file) {
	static const char OUTPUT_DIRECTORY[] = "..\\output\\";
	int length = strlen(OUTPUT_DIRECTORY) + strlen(input_file) + 5;
	char * output_file_path = (char *) calloc(length, sizeof(char));
	
	strcpy_s(output_file_path, length, OUTPUT_DIRECTORY);
	strcat_s(output_file_path, length, input_file);

	return output_file_path;
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
	matrix = generate_matrix(column_count, row_count);
	
	// calloc 0's the array for us, not needed but still nice
	column_costs = (int *)calloc(column_count, sizeof(int));

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
			matrix[row][covering_column - 1] = 1;
		}
	}

	instance->column_costs = column_costs;
	instance->column_count = column_count;
	instance->row_count = row_count;
	instance->instance_matrix = matrix;
}


