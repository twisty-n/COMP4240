#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include "constructive_heuristic.h"

#define TRUE 1
#define FALSE 0

typedef int boolean;

void random_construction(Instance * instance, Solution * solution) {

	// First we create an array for the covering columns
	// This will be at most the number of rows, most likely less
	// The ith row will be covered by the value
	int * coverings = (int *) calloc(instance->row_count, sizeof(int));
	int * minimal_coverings = (int *) calloc(instance->row_count, sizeof(int));

	// Keep a track of the current cost
	int current_cost = 0;
	
	// The number of unique columns required to cover all rows
	int number_of_coverings = 0;

	// For each row in the problem instance, select a random one 
	// column from the covering columns for that row that covers the solution
	for (int row = 0; row < instance->row_count; row++) {
		
		int selected_column;
		int coverings_for_row = instance->row_covering_count[row];
		int * column_covers_for_row = instance->raw_coverings[row];
		srand(time(NULL)); // Seed the RNG

		// Now pick the random column - using notation as per http://www.c-faq.com/lib/randrange.html
		// selected_column = column_covers_for_row[(int)((double)rand() / (((double)RAND_MAX + 1) * coverings_for_row))];
		selected_column = column_covers_for_row[rand() % coverings_for_row];
		coverings[row] = selected_column;

		// Determine if the selected column exists in the current set of minimal coverings
		boolean column_exists = FALSE;
		for (int i = 0; i < number_of_coverings; i++) {
			//printf("selected solumn %d __ min: %d", selected_column, minimal_coverings[i]);
			if (selected_column == minimal_coverings[i]) {
				column_exists = TRUE;
				break;
			}
		}

		// If the column isn't in that set, add it, and its cost
		if (!column_exists) {
			minimal_coverings[number_of_coverings] = selected_column;
			current_cost += instance->column_costs[selected_column];
			number_of_coverings++;
		}
	}

	solution->cost = current_cost;
	solution->covering_columns = coverings;
	solution->minimal_cover = minimal_coverings;
	solution->number_of_covers = number_of_coverings;
}
