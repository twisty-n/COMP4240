#include "stdafx.h"
#include "local_search.h"
#include "constructive_heuristic.h"
#include "utilities.h"
#include <stdlib.h>
#include <time.h>


#define TRUE 1
#define FALSE 0
#define NEIGHBOURHOOD_SIZE 10		//currently has 10 solutions in a neighbourhood, can ammend if required


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

	int K = 50; // Because why not. 
	
	// Persist the solution as it currently is 
	Solution current_best = deep_copy(instance, solution);
	for (int i = 0; i < repetitions; i++, remove_redundant_column = TRUE) {

		Solution working_solution = deep_copy(instance, &current_best);
		while (remove_redundant_column) {

			// The first step in the process is to select a set at random, and 
			// "remove" it from the solution. 
			int column_to_remove = working_solution.minimal_cover[rand() % solution->number_of_covers];
			
			// TODO: Clean up after myself!!!!!
			working_solution.columns_in_solution[column_to_remove] = FALSE;
			int cost_of_removed = instance->column_costs[column_to_remove];
			working_solution.cost -= cost_of_removed;

			// TODO: Update minimal_covers
			// Covering columns
			
			// If the new solution is valid
			if (is_feasible(instance, &working_solution)) {
				// Find the updated, without the removed column and update the solution cost
				// Update the solution with the removed column
				remove_redundant_column = TRUE;
			}
			else {
				// Otherwise, leave the removed column from the solution and continue
				remove_redundant_column = FALSE;
			}
		}

		// 
		for (int i = 0; i < K; i++) {
			// Randomly select a column from one of the unselected columns
			int column_to_add = working_solution.non_covering_columns[rand() % solution->number_of_non_covering];
			// TODO:
			// Update minimal cover, non-covering columns and all that
			working_solution.columns_in_solution[column_to_add] = TRUE;
			working_solution.cost += instance->column_costs[column_to_add];
			// See if the solution with this column, and still the one removed from before 
			// is a valid solution
			int compare_result = compare(&working_solution, &current_best);
			if (is_feasible(instance, &working_solution) && ((compare_result == 0) || (compare_result == 1))) {
				// This good
				// Working solution is up to date 
				current_best = working_solution;
			}
			else {
				// Roll back changes
				// TODO:
				// Update minimal cover, non-covering columns and all that
				working_solution.columns_in_solution[column_to_add] = FALSE;
				working_solution.cost -= instance->column_costs[column_to_add];
			}
		}
	}

	// Lets hope this doesn't screw itself
	solution = &current_best;
	remove_redundant_column = TRUE;
}


void perform_local_search_first_accept(Instance * instance, Solution * solution, time_t start_sol) {
	
	//construct a random solution S0
	//TODO:  ensure time is added to the solution properly, so that deep copy will work.
	random_construction(instance, solution);
	time_t end_sol = get_current_time();
	solution->time = difftime(end_sol, start_sol);
	
	//go to local_search
	local_search_first_accept(instance, solution);
}


void local_search_first_accept(Instance * instance, Solution * solution_S0) {	

	//define the neighbourhood
	struct Solution neighbourhood[NEIGHBOURHOOD_SIZE];
	for (int i = 0; i < NEIGHBOURHOOD_SIZE; i++) {
		neighbourhood[i] = randomly_generate_neighbour(instance, solution_S0);
	}
}

Solution randomly_generate_neighbour(Instance * instance, Solution * solution_S0) {

	int total_rows_to_swap = 10;				
	int non_feasible = 0; 
	int non_feasibile_max = 15;
	boolean solution_found = FALSE;
	
	srand(time(NULL));					// Seed the RNG

	Solution neighbour = deep_copy(instance, solution_S0);

	for (int i = 0; i < total_rows_to_swap; i++) {

		//pick a random row from the minimal coverings

		//pick a random row from the non-covering

		//Swap rows
	}
	//check feasibility

	return neighbour;
}