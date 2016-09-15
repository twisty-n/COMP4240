#include "stdafx.h"
#include "problem_instance.h"
#include "matrix.h"
#include <stdlib.h>


void free_instance(Instance * instance) {
	free_matrix(instance->instance_matrix, instance->row_count);
	free(instance->column_costs);
}

void print_instance(Instance * instance) {
	printf("Number of rows: %d\nNumber of columns: %d\n", instance->row_count, instance->column_count);
	for (int i = 0; i < instance->column_count; i++) {
		printf("#%d:%d ", i + 1, instance->column_costs[i]);
	}
	printf("\n");
	print_matrix(instance->instance_matrix, instance->column_count, instance->row_count);
}