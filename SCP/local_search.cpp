#include "stdafx.h"
#include "local_search.h"
#include "constructive_heuristic.h"
#include "utilities.h"
#include <stdlib.h>
#include <time.h>


#define TRUE 1
#define FALSE 0
#define NEIGHBOURHOOD 10		//currently has 10 solutions in a neighbourhood, can ammend if required


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
			
			// if it isn't, go back to 55 otherwise
			// Find the new cost. If the cost is better, maintain it, and the new solution
			// then go back to fifty three

		}
		

	}
}


void perform_local_search_first_accept(Instance * instance, Solution * solution) {
	
	//construct a random solution S0
	//TODO:  ensure time is added to the solution properly, so that deep copy will work.
	random_construction(instance, solution);
	
	//go to local_search
	local_search_first_accept(instance, solution);
}


void local_search_first_accept(Instance * instance, Solution * solution_S0) {	

	for (int i = 0; i < NEIGHBOURHOOD; i++) {
		


	}
}

Solution * randomly_generate_neighbour(Instance * instance, Solution * solution_S0) {

	int total_rows_to_swap = 10;
	Solution neighbour = deep_copy(instance, solution_S0);


	for (int i = 0; i < total_rows_to_swap; i++) {


	}

	return &neighbour;
}