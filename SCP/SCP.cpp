/**
 * Program will operate on single instances of the set covering problem
 * Arguments:
 *		1. string:			Filename representing the input file to deal with
 *		2. string:			Represents the operation (section of part 2 of the assignment) to perform
 *		3. int:optional:	The current best known solution for the given input file (only required
 *							if finding a solution and second argument is a search single-point or 
 *					        metaheurstic)
 */

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Custom includes */
#include "matrix.h"
#include "problem_instance.h"
#include "solution.h"
#include "constructive_heuristic.h"

/* Constants */
#define FILE_OPEN_SUCCESS 0
#define NO_FILE_FOUND_ERROR_CODE "error_code:1:no_file_found"
#define INVALID_ARGUMENTS_ERROR_CODE "error_code:2:invalid_arguments"
#define NO_OF_RUNS 3				//TODO:  remove this. It is a hack until cmd line argument defines the number of runs for each heuristic
#define TRUE 1
#define FALSE 0


typedef int boolean;

/* Function prototypes */

boolean valid_arguments(int argument_count, char * arguments[]);
void generate_problem_instance(Instance * instance, FILE * file);
char * generate_output_file_path(char * input_file);
time_t print_current_time();

int main(int argument_count, char * argv[])
{
	if (!valid_arguments(argument_count, argv)) {
		return 0;
	}

	boolean print_raw_output = FALSE;
	FILE * file;
	FILE * output_file;
	char * input_file_name = argv[1];
	//char * input_file_name = "21scp61.txt";
	//printf("loading file name %s\n", input_file_name);

	char * output_file_path = generate_output_file_path(input_file_name);
	errno_t file_open_status = fopen_s(&file, input_file_name, "r");

	// Ensure that the input file opened correctly
	if (file_open_status != FILE_OPEN_SUCCESS) {
		printf("%s Opening input file %s failed\n", NO_FILE_FOUND_ERROR_CODE, input_file_name);
		return 0;
	}
	
	Instance instance;
	Solution current_solution;
	Solution best_solution;
	

	//printf("Generating problem instance started "); 
	time_t start_formulate = print_current_time(); //printf("\n");

	generate_problem_instance(&instance, file);

	//printf("Generating problem instance complete ");
	time_t end_formulate = print_current_time(); //printf("\n");

	fclose(file);

	//printf("Generating problem instance took %f seconds\n", difftime(end_formulate, start_formulate));
	//printf("Outputing instance representation to file\n");

	//Open a file for output, write then close
	//fopen_s(&output_file, output_file_path, "w");
	//print_instance_to_file(&instance, output_file);
	//fclose(output_file);

	int total_cost = 0;
	double total_time = 0;
	for (int i = 0; i < NO_OF_RUNS; i++) {
		//print_instance(&instance);
		//printf("Solution generated started ");
		time_t start_sol = print_current_time(); //printf("\n");
		
		//random_construction(&instance, &current_solution);
		greedy_construction(&instance, &current_solution, FALSE);	//TRUE == unicost, FALSE == NON-UNICOST

		//printf("Solution generation complete");
		time_t end_sol = print_current_time(); //printf("\n");
		//printf("Generating a solution took %f seconds", difftime(end_sol, start_sol));
		current_solution.time = difftime(end_sol, start_sol);

		if (i == 0) {
			best_solution = current_solution;
		}
		else if (current_solution.cost < best_solution.cost) {
			best_solution = current_solution;
		}
		total_cost += current_solution.cost;
		total_time += current_solution.time;
	}

	
	double average_cost = (double)total_cost / NO_OF_RUNS;
	double average_time = total_time / NO_OF_RUNS;

	if (NO_OF_RUNS <= 1) {
		print_solution_stats(&best_solution);
	}
	else {
		print_solution_stats(&best_solution, average_cost, average_time);
	}


	if (print_raw_output) {
		fopen_s(&output_file, output_file_path, "w");
		print_solution_to_file(&best_solution, output_file, "random_construction");
		fclose(output_file);
	}

	// TODO: Free all the memory we allocated
	free(output_file_path);
	free_instance(&instance);
	free_solution(&current_solution);
	free_solution(&best_solution);
	

	return 0;
}

time_t print_current_time() {
	// Get the current time
	time_t now = time(0);
	struct tm time_val;
	localtime_s(&time_val, &now);

	char time_string[9];
	strftime(time_string, sizeof(time_string), "%H:%M:%S", &time_val);

	//printf("%s", time_string);

	return now;
}

char * generate_output_file_path(char * input_file) {
	static const char OUTPUT_DIRECTORY[] = "..\\output\\raw_dump\\";
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
	int ** raw_coverings;
	int * column_costs;
	int * row_covering_count;
	
	// Read the rows and columns for the input and generate the basix matrix
	fscanf_s(file, "%d%d", &row_count, &column_count);
	matrix = generate_matrix(column_count, row_count);
	raw_coverings = generate_matrix(row_count);
	
	// calloc 0's the array for us, not needed but still nice
	column_costs = (int *)calloc(column_count, sizeof(int));
	row_covering_count = (int *)calloc(column_count, sizeof(int));

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
		
		// Maintain information about the number of coverings for a given row
		// and which columns presented that cover
		row_covering_count[row] = coverings;
		raw_coverings[row] = (int *) calloc(coverings, sizeof(*raw_coverings[row]));

		// Now iterate over the coverings, flipping the needed 'bits'
		for (int column_count = 0; column_count < coverings; column_count++) {
			fscanf_s(file, "%d", &covering_column);
			matrix[row][covering_column - 1] = 1;
			raw_coverings[row][column_count] = covering_column - 1;
		}
	}

	instance->column_costs = column_costs;
	instance->raw_coverings = raw_coverings;
	instance->row_covering_count = row_covering_count;
	instance->column_count = column_count;
	instance->row_count = row_count;
	instance->matrix = matrix;
}


