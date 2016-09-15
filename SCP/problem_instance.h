#pragma once

struct Instance {
	
	// A full matrix representing the given coverings
	int ** matrix;

	// A grid in which a raw_coverings[x] represents the xth row and raw_coverings[x][y] represents a column with a cost
	int ** raw_coverings;

	// An array in which row_covering_count[x] represents the number of coverings for column x
	int * row_covering_count;

	// An array in which column_costs[x] gives the cost of column x (after accounting for col 1 -> col 0)
	int * column_costs;

	// Number of rows in the instance matrix
	int row_count;

	// Number of columns in the instance matrix
	int column_count;
};

typedef struct Instance Instance;

/*
 * De-allocates the memory associated with a problem instance
 */
extern void free_instance(Instance * instance);

/*
 * Prints the information associated with a given problem instance
 */
extern void print_instance(Instance * instance);

/*
 * Prints the instance information to the given file
 */
extern void print_instance_to_file(Instance * instance, FILE * file);
