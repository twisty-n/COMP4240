#pragma once

struct Instance {
	int ** instance_matrix;
	int * column_costs;
	int row_count;
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
