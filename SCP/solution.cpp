#include "stdafx.h"
#include <stdlib.h>
#include "solution.h"
#include "utilities.h"

void print_solution(Instance * instance, Solution * solution) {
	printf("\n");
	printf("Total Cost: %d\n", solution->cost);
	printf("Required Covers %d\n", solution->number_of_covers);
	printf("Minimal Covering Columns: \n");
	print_array_real_column_labels(solution->minimal_cover, solution->number_of_covers);
	printf("Non covering Covering Columns: \n");
	print_array_real_column_labels(solution->non_covering_columns, solution->number_of_non_covering);
	printf("1 indicates row is covered by col:  \n");
	print_array(solution->columns_in_solution, instance->row_count);
	printf("\n");
	printf("indicates which col is covering which row:  \n");
	print_array_real_column_labels(solution->covering_column, instance->row_count);
	printf("\n");
}

void print_solution_stats(Solution * solution) {
	printf("%d ",solution->cost);		//prints basic info so python can add to the exec summary
	printf("%6.4lf",solution->time);
}

void print_solution_stats(Solution * solution, double average_cost, double average_time) {
	printf("%d ", solution->cost);		//prints basic info so python can add to the exec summary
	printf("%6.6lf ", solution->time);
	printf("%5.2lf ", average_cost);
	printf("%6.6lf ", average_time);
}

void print_solution_to_file(Solution * solution, FILE * file, char * heuristic) {
	fprintf(file, "best solution for heuristic %s", heuristic);
	fprintf(file, "Total Cost: %d\n", solution->cost);
	fprintf(file, "Required Covers %d\n", solution->number_of_covers);
	fprintf(file, "Minimal Covering Columns: \n");
	for (int i = 1; i <= solution->number_of_covers; i++) {
		// Plus 1 so that the columns are displayed sensibly
		fprintf(file, "%5d ", solution->minimal_cover[i - 1] + 1);
		if (i % 6 == 0 && i != 0) {
			// Prints 6 per row (arbitarily) to fit into the console window
			fprintf(file, "\n");
		}
	}
}

void free_solution(Solution * solution) {
	free(solution->minimal_cover);
	free(solution->covering_column);
	free(solution->columns_in_solution);
}

