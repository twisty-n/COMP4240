#include "stdafx.h"
#include "problem_instance.h"
#include "matrix.h"
#include <stdlib.h>


void free_instance(Instance * instance) {
	free_matrix(instance->instance_matrix, instance->row_count);
	free(instance->column_costs);
}

void print_instance(Instance * instance) {
	printf("Number of rows: %d\nNumber of columns: %d", instance->row_count, instance->column_count);
	printf("\nColumn:\t");
	for (int i = 0; i < instance->column_count; i++) {
		printf(" %5d ", i + 1);
	}
	printf("\nCost:\t");
	for(int i = 0; i < instance->column_count; i++) {
		printf("%4d", instance->column_costs[i]);
	}
	printf("\n");
	for (int num_rows = 0; num_rows < instance->row_count; num_rows++) {
		printf("Row %d:\t", num_rows + 1);
		for (int num_cols = 0; num_cols < instance->column_count; num_cols++) {
			printf("%4d", instance->instance_matrix[num_rows][num_cols]);
		}
		printf("\n");
	}
}

void print_instance_to_file(Instance * instance, FILE * file) {
	fprintf(file, "Number of rows: %d\nNumber of columns: %d", instance->row_count, instance->column_count);
	fprintf(file, "\nColumn:\t");
	for (int i = 0; i < instance->column_count; i++) {
		fprintf(file, " %5d", i + 1);
	}
	fprintf(file, "\nCost:\t");
	for(int i = 0; i < instance->column_count; i++) {
		fprintf(file, "%4d", instance->column_costs[i]);
	}
	fprintf(file, "\n");
	for (int num_rows = 0; num_rows < instance->row_count; num_rows++) {
		fprintf(file, "Row %d:\t", num_rows + 1);
		for (int num_cols = 0; num_cols < instance->column_count; num_cols++) {
			fprintf(file, "%4d", instance->instance_matrix[num_rows][num_cols]);
		}
		fprintf(file, "\n");
	}
}