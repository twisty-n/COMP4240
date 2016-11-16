#pragma once

#include "stdafx.h"
#include "problem_instance.h"
#include "solution.h"

#define TRUE 1
#define FALSE 0

typedef int boolean;


/*
	If an array needs to be sorted - lowest to highest
	follows: http://www.algolist.net/Algorithms/Sorting/Quicksort
*/
void quick_sort(int * array, int p, int r);

/*
	quick_sorts 2 arrays working in parallel
	index and value will be sored together, 
	based on the value (lowest to highest)
*/
void quick_sort(int * index, int * value, int left, int right);

/*
	will return ture is all rows are covered by a column
*/
boolean is_feasible(Instance * instance, Solution * solution);

/*
	will compare the cost of 2 solutions
	reutrns:
	-1 if solution a is cheaper than solution b
	0 if equal
	1 if solution b is cheaper than solution a
	
*/
int compare(Solution * solution_a, Solution * solution_b);

/*
	returns a deep_copy of the solution.
*/
Solution deep_copy(Instance * instance, Solution * solution_s0);

/*
	returns a deep copy of an array
*/
int * copy_array(int * array, int size);

/*
	sets a calloced array to -1's
*/
void set_to_minus_ones(int * array, int size);


/*
	returns the current time
*/
time_t get_current_time();

/*
	will print the details of an array
*/
void print_array(int * array, int size);

/*
	will print the details of an array
	incrementing the output by 1 (use this for presenting data (columns/rows) from the matrix)
*/
void print_array_real_column_labels(int * array, int size);

/*
	will generate a random number between 0 and 1
*/
double bounded_rand();





