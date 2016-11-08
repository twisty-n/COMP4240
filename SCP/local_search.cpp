#include "local_search.h"
#include "constructive_heuristic.h"
#include "utilities.h"
#include <stdlib.h>
#include <time.h>

#define FALSE 0

void perform_local_search_1(Instance * instance, Solution * solution) {
	// First we'll obtain an initial solution using a greedy approach
	greedy_construction(instance, solution, FALSE);

	// Then we'll actually perform the local search given our greedy instance
	local_search_1(instance, solution);
}


void local_search_1(Instance * instance, Solution * solution) {
	// We need to keep a track of the best found cost, and the current state of the solution
	// This is already done, as being stored in the solution file. 
	
	// Now we need to figure out the number of iterations to run the thing gor
	// from what I can make out, it is the number of elements times the 
	// number of columns in the soltuion
	int repetitions = instance->row_count * solution->number_of_covers;
	srand(time(NULL));
	boolean remove_redundant_column = TRUE;
	for (int i = 0; i < repetitions; i++, remove_redundant_column = TRUE) {

		while (remove_redundant_column) {

			// The first step in the process is to select a set at random, and 
			// "remove" it from the solution. 

			// If the new solution is valid
			if (TRUE) {
				// Find the updated, without the removed column and update the solution cost
				// Update the solution with the removed column
			}
			else {
				// Otherwise, leave the removed column from the solution and continue
			}

		}

		// 
		for (int i = 0; i < K; i++) {
			// Randomly select a column from one of the unselected columns
			
			// See if the solution with this column, and still the one removed from before 
			// is a valid solution

		}
		

	}
}