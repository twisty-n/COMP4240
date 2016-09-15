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

/* Constants */
#define FILE_OPEN_SUCCESS 0
#define NO_FILE_FOUND_ERROR_CODE "error_code:1 "


int main(int argument_count, char * argv[])
{
	FILE * f;
	char * input_file_name = "scp_input_1";
	errno_t file_open_status = fopen_s(&f, "", "r");

	int row_count = 5, column_count = 4;

	int ** test = generate_matrix(row_count, column_count);

	zero_matrix(test, column_count, row_count);

	print_matrix(test, column_count, row_count);

	free_matrix(test, row_count);

	return 0;

	

	// Ensure that the input file opened correctly
	if (file_open_status != FILE_OPEN_SUCCESS) {
		printf("%s Opening input file %s failed", NO_FILE_FOUND_ERROR_CODE, input_file_name);
		return 0;
	}




    return 0;
}


