#pragma once

#include "problem_instance.h"


struct Solution {
	int cost;

	/*
		Each cell represents a row, the value represents the column covering that row
		-1 indicates that the row is not covered

		//todo - I can see a bug with using this - this only shows one column that is covering
		//that row and does not account for the other colums in the solution which may also be 
		//covering that row.  We only update this one time.

		//I will update a feature in the neighborhood generation to ensure that this is
		//kept up-to-date based on checking the other columns which are still in the
		//solution.
	*/
	int * covering_columns;

	/*
		An array holding the minimal columns required for the solution
		Holds the same information as columns_in_solution, but is 
		more of a list, where the unfilled indexes are not part of the solution.
		array is the size of the rows in the solution.  -1 is flag which indicates
		the end of the covering olumns.  Can also be managed using the logical
		size, number_of_covers.
	*/
	int * minimal_cover;
	
	/*
		An array holding the non_covering_columns listed the solution
		holds the same information as columns_in_solution, but is more of
		a list, where the filled indexes are not part of this list.  Is the size of
		the rows in the instance. -1 indicates end of valid columns.  Can also
		be managed by the logical size number_of_non_covering.
	*/
	int * non_covering_columns;		
	/*
		Each cell represents a column, a value of 1 indicates a column is in the solution
		a value of -1 indicates a column is outside of the solution
	*/
	int * columns_in_solution;

	/*
		The number of columns in the solution (useful for unicost SCP)
	*/
	int number_of_covers;
	
	/*
		The number of columns not in the solution
	*/
	int number_of_non_covering;
	
	/*
		The runtime to generate this solution
	*/
	double time;
};

typedef struct Solution Solution;

void print_solution(Instance * instance, Solution * solution);

void print_solution_stats(Solution * solution);

void print_solution_stats(Solution * solution, double average_cost, double average_time);

void free_solution(Solution * solution, FILE * file);

void free_solution(Solution * solution);

void print_solution_to_file(Solution * solution, FILE * file,  char * heuristic);