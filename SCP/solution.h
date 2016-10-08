#pragma once

struct Solution {
	int cost;

	/*
		Each cell represents a row, the value represents the column covering that row
	*/
	int * covering_columns;

	/*
		An array holding the minimal columns required for the solution
		Holds the same information as columns_in_solution, but is 
		more of a list, where the unfilled indexes are not part of the solution
	*/
	int * minimal_cover;

	/*
		Each cell represents a column, a value of 1 indicates a column is in the solution
		a value of 0 indicates a column is outside of the solution
	*/
	int * columns_in_solution;

	/*
		The number of columns in the solution (useful for unicost SCP)
	*/
	int number_of_covers;

	/*
		The runtime to generate this solution
	*/
	double time;
};

typedef struct Solution Solution;

void print_solution(Solution * solution);

void print_solution_stats(Solution * solution);

void print_solution_stats(Solution * solution, double average_cost, double average_time);

void free_solution(Solution * solution, FILE * file);

void print_solution_to_file(Solution * solution, FILE * file,  char * heuristic);