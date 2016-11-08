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

	for (int i = 0; i < repetitions; i++) {

		// The first step in the process is to select a set at random, and 
		// remove it from the solution. 
		

	}
}

void local_search_first_accept(Instance * instance, Solution * solution) {

	//construct a random solution S0



}