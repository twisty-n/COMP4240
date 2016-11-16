/**
 * Program will operate on single instances of the set covering problem
 * Arguments:
 *		1. string:			Filename representing the input file to deal with
 *		2. int:				Represents the operation (section of part 2 of the assignment) to perform
 *							1.  Random Constructive
 *							2.  Greedy Constructive
 *							3.  Local search 1 (tba)
 *							4.  Local search 2 (tba)
 *							5.  Single Point meta-heuristic
 *							6.  Population based meta-heuristic
 *		3. int				Number of runs
 *      4. int				Will be used to indicate debug mode. 1 == TRUE 0 == FALSE		
 *		5. int:optional:	The current best known solution for the given input file (only required
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
#include "local_search.h"
#include "simulated_annealing.h"
#include "utilities.h"
#include "jpso.h"

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
char * generate_output_file_path(char * input_file);
char * generate_debug_file_path(char * input_file, char * h_code);

int main(int argument_count, char * argv[])
{
	
	srand(time(NULL));								//Seed the RNG for the program

	//validate number of arguments
	if (!valid_arguments(argument_count, argv)) {
		printf("Invalid arguments");
		return 0;
	}

	boolean print_raw_output = FALSE;				//TODO:  Keep this set as false when running from python until issue #5 on github is resolved.
	FILE * file;
	FILE * output_file;
//	FILE * debug_log;								//adhoc use during development to help with debug.  currently being used by greedy.
	char * input_file_name;

	boolean debug = atoi(argv[4]);
	if (debug) {
		input_file_name = "15scp55.txt";				//use this when running in debug mode.  Make sure arg2 and arg3 for VS2015 have values you can run with.
	}
	else {
		input_file_name = argv[1];
	}
	char * output_file_path = generate_output_file_path(input_file_name);
//	char * debug_file_path = generate_debug_file_path(input_file_name, argv[2]);
//	fopen_s(&debug_log, debug_file_path, "w");


	errno_t file_open_status = fopen_s(&file, input_file_name, "r");	
	// Ensure that the input file opened correctly
	if (file_open_status != FILE_OPEN_SUCCESS) {
		printf("%s Opening input file %s failed\n", NO_FILE_FOUND_ERROR_CODE, input_file_name);
		return 0;
	}

	int operation_mode = atoi(argv[2]);
	//validate opreation mode
	if (operation_mode < 0 || operation_mode > 6) {
		printf("Invalid operation_mode %d", operation_mode);
		return 0;
	}
	char * operation = "";					//will be used populated and used later in program for raw output identification

	int number_of_runs = atoi(argv[3]);
	//validate number of runs
	if (number_of_runs < 0) {
		printf("Invalid number of runs");
		return 0;
	}
	
	Instance instance;
	Solution current_solution;				//keeps track of the current solution from multiple runs
	Solution best_solution;					//holds the best solution found this program
	Solution * returned;					//holds the best solution returned from the local searches or heuristic options
	
	time_t start_formulate = get_current_time();
	generate_problem_instance(&instance, file);
	time_t end_formulate = get_current_time();
	fclose(file);

	int total_cost = 0;
	double total_time = 0;
	
	for (int i = 0; i < number_of_runs; i++) {

		time_t start_sol = get_current_time();
		current_solution.time = start_sol;
		
		switch (operation_mode) {
		case 1:
			random_construction(&instance, &current_solution);
			operation = "random_construction";
			break;
		case 2:			
			greedy_construction(&instance, &current_solution, FALSE);	//TRUE == unicost, FALSE == NON-UNICOST
			operation = "greedy_construction";
			break;
		case 3:
			greedy_construction(&instance, &current_solution, FALSE);	//TRUE == unicost, FALSE == NON-UNICOST
			//printout required for python report
			if (i == 0) {
				printf("%d ", current_solution.cost);
			}
			returned = local_search_best_accept(&instance, &current_solution);
			operation = "local_search_best_accept";
			current_solution = *returned;
			break;
		case 4:
			random_construction(&instance, &current_solution);	//TRUE == unicost, FALSE == NON-UNICOST
			//printout required for python report
			if (i == 0) {
				printf("%d ", current_solution.cost);
			}
			returned = local_search_first_accept(&instance, &current_solution);
			operation = "local_search_first_accept";
			current_solution = *returned;
			break;
		case 5:
			random_construction(&instance, &current_solution);	//TRUE == unicost, FALSE == NON-UNICOST
			//printout required for python report
			if (i == 0) {
				printf("%d ", current_solution.cost);
			}
			returned = perform_simulated_annealing(&instance, &current_solution);
			operation = "simulated_annealing";
			current_solution = *returned;
			break;
		case 6:
			current_solution = jpso(&instance, 10);
			operation = "meta_jpso";
			break;
		}
		
		time_t end_sol = get_current_time();
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

	double average_cost = (double)total_cost / number_of_runs;
	double average_time = total_time / number_of_runs;
	if (number_of_runs == 1) {
		print_solution_stats(&best_solution);
	}
	else {
		print_solution_stats(&best_solution, average_cost, average_time);
	}

	if (print_raw_output) {
		fopen_s(&output_file, output_file_path, "w");
		print_solution_to_file(&best_solution, output_file, operation);
		fclose(output_file);
	}
	free(output_file_path);
	free_instance(&instance);
	free_solution(&best_solution);

	return 0;
}

char * generate_output_file_path(char * input_file) {
	static const char OUTPUT_DIRECTORY[] = "..\\output\\raw_dump\\";
	int length = strlen(OUTPUT_DIRECTORY) + strlen(input_file) + 5;
	char * output_file_path = (char *) calloc(length, sizeof(char));
	
	strcpy_s(output_file_path, length, OUTPUT_DIRECTORY);
	strcat_s(output_file_path, length, input_file);

	return output_file_path;
}

char * generate_debug_file_path(char * input_file, char * h_code)
{
	static const char DEBUG_DIRECTORY[] = "..\\DEBUG\\";
	int length = strlen(DEBUG_DIRECTORY) + strlen(input_file) + 7;
	char * debug_file_path = (char *)calloc(length, sizeof(char));

	strcpy_s(debug_file_path, length, DEBUG_DIRECTORY);
	strcat_s(debug_file_path, length, h_code);
	strcat_s(debug_file_path, length, "_");
	strcat_s(debug_file_path, length, input_file); 

	return debug_file_path;
}

// Checks the provided arguments, at minimum there should be a filename
// an operation mode, and the number of times you wish to run this heursitic for this file
boolean valid_arguments(int argument_count, char * arguments[]) {
	//printf("%d %s", argument_count, arguments[0]);
	if (argument_count < 5) {
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


