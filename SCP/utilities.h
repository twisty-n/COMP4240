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
	inputs:	 array to be sorted
			 p (first index, use 0 when calling)
			 r (last index, use the final idex of the sort region when calling).
*/
void quick_sort(int * array, int p, int r);

int * reverse_quick_sort(int * array, int p, int r, int size);

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
	incrementing the output by 1
	(use this for presenting columns/rows from the matrix)
*/
void print_array_real_column_labels(int * array, int size);

/*
	will generate a random number between 0 and 1
*/
double bounded_rand();

/*
	will print array contents to screen, then run quicksort (parrallel version)
	then print the results to screen.  For devel use.
	intputs: array to be sorted - indexs
			 array to be sotred (based on these values)
			 left (first index, use 0 when calling)
			 right (last index, use the final idex of the sort region when calling).
*/
void test_arrays_and_quick_sort(int * index, int * value, int left, int right);

/*
	will take an array and exapand it by 1 empty element
*/
int * expand_array(int * array, int size);

/*
	Removes a column from the solution and updates all necessary fields
*/
void remove_column(Instance * instance, Solution * target, int candidate);

/*
	This nice chap will add a column to your solution and udpate any required details
*/
void add_column(Instance * instance, Solution * target, int candidate);

int sanity_cost(Instance * instance, Solution * solution);



