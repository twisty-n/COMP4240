#include "stdafx.h"
#include "local_search.h"
#include "constructive_heuristic.h"
#include "utilities.h"
#include <stdlib.h>
#include <time.h>


#define TRUE 1
#define FALSE 0
#define NOT_COVERED 0
#define COVERED 1
#define NEIGHBOURHOOD_SIZE 10		//currently has 10 solutions in a neighbourhood, can ammend if required


Solution * perform_local_search_best_accept(Instance * instance, Solution * solution) {
	// First we'll obtain an initial solution using a greedy approach
	greedy_construction(instance, solution, FALSE);
	//random_construction(instance, solution);

	printf("%d", solution->cost);			//required for output to python/excel

	// Then we'll actually perform the local search given our greedy instance
	return local_search_best_accept(instance, solution);
}


Solution * local_search_best_accept(Instance * instance, Solution * solution) {
	// We need to keep a track of the best found cost, and the current state of the solution
	// This is already done, as being stored in the solution file. 
	
	// Now we need to figure out the number of iterations to run the thing gor
	// from what I can make out, it is the number of elements times the 
	// number of columns in the soltuion

	int repetitions = 200; // instance->row_count * solution->number_of_covers;
	boolean remove_redundant_column = TRUE;

	int K = 1000; //Because why not. 
	
	// Persist the solution as it currently is 
	Solution current_best = deep_copy(instance, solution);

	for (int i = 0; i < repetitions; i++, remove_redundant_column = TRUE) {

		Solution working_solution = deep_copy(instance, &current_best);
		
		//Goal of part one is to remove columns from the solution until an unfeasible solution is generated.
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
		int current_repetition = i;
		//Goal of part two is to randombly add a column from the non-covering columns
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

			//printf("Iteration: %d. Hill Climb Iteration: %d. Current best cost: %d. Working best cost: %d \n", current_repetition, i, current_best.cost, working_solution.cost);

			if (is_feasible(instance, &working_solution) && ((compare_result == 0) || (compare_result == -1))) {
				// This good
				// Working solution is up to date
				//printf("Iteration: %d. Hill Climb Iteration: %d. Current best cost: %d. Working best cost: %d \n", current_repetition, i, current_best.cost, working_solution.cost);
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
	// update LC:  lol - it totally screwed itself the first time :P
	solution = &current_best;
	remove_redundant_column = TRUE;

	return solution;
}


Solution * perform_local_search_first_accept(Instance * instance, Solution * solution, time_t start_sol) {
	
	//construct a random solution S0
	//greedy_construction(instance, solution, FALSE);
	random_construction(instance, solution);
	printf("%d ", solution->cost);			//required for output to python/excel
	time_t end_sol = get_current_time();
	solution->time = difftime(end_sol, start_sol);
	
	//go to local_search
	return local_search_first_accept(instance, solution);
}


Solution * local_search_first_accept(Instance * instance, Solution * solution_S0) {	

	Solution current = *solution_S0;
	Solution best_neighbour;
	boolean terminate_search = FALSE;
	struct Solution neighbourhood[NEIGHBOURHOOD_SIZE];

	while (!terminate_search) {
		
		best_neighbour = current;

		//define the neighbourhood	
		for (int i = 0; i < NEIGHBOURHOOD_SIZE; i++) {
			neighbourhood[i] = randomly_generate_neighbour(instance, &current);
		}

		//for each solution in the neighbourhood
		for (int i = 0; i < NEIGHBOURHOOD_SIZE; i++) {
			//check costs, if you found a better guy, then set as your best neighbour
			if (compare(&neighbourhood[i], &best_neighbour) == -1){
				best_neighbour = neighbourhood[i];
				break;
			}
		}

		//if it turns out the current solution happens to be the best solution you already had in your neighbourhood
		if(compare(&current, &best_neighbour) == 0) {
			terminate_search = TRUE;
		}
		else {
			current = best_neighbour;
		}
	}//end while

	return &best_neighbour;
}

Solution randomly_generate_neighbour(Instance * instance, Solution * solution_S0) {

	int total_rows_to_swap = 5;				
	int non_feasible = 0;						//counter for the number of non-feasible solutions generated
	int non_feasible_max = 500;					//max number of non-feasible solutions allowed.  When maxed, return s0.
	boolean solution_found = FALSE;
	Solution neighbour = deep_copy(instance, solution_S0);

	

	while (!solution_found && non_feasible < non_feasible_max) {
		for (int i = 0; i < total_rows_to_swap; i++) {
			//pick a random row from the minimal coverings
			int column_to_remove_index = rand() % neighbour.number_of_covers;
			int column_to_remove = neighbour.minimal_cover[column_to_remove_index];

			//pick a random row from the non-covering
			int column_to_add_index = rand() % neighbour.number_of_non_covering;
			int column_to_add = neighbour.non_covering_columns[column_to_add_index];

			//Swap cols in the solution
			neighbour.non_covering_columns[column_to_add_index] = column_to_remove;
			neighbour.minimal_cover[column_to_remove_index] = column_to_add;

			//updates
			neighbour.columns_in_solution[column_to_add] = COVERED;
			neighbour.columns_in_solution[column_to_remove] = NOT_COVERED;
			neighbour.cost -= column_to_remove;
			neighbour.cost += column_to_add;

		}
		if (is_feasible(instance, &neighbour)) {
			solution_found = TRUE;
		}
		else {
			non_feasible++;
		}
	}

	return neighbour;
}

/*
	still in development - need to work out the plan for this one - how to generate neighbours
*/
Solution not_so_randomly_generate_neighbour(Instance * instance, Solution * solution_S0) {

	int total_rows_to_swap = 5;
	int non_feasible = 0;						//counter for the number of non-feasible solutions generated
	int non_feasible_max = 2000;				//max number of non-feasible solutions allowed.  When maxed, return s0.
	boolean solution_found = FALSE;
	Solution neighbour = deep_copy(instance, solution_S0);



	while (!solution_found && non_feasible < non_feasible_max) {
		for (int i = 0; i < total_rows_to_swap; i++) {
			//pick a random row from the minimal coverings
			int column_to_remove_index = rand() % neighbour.number_of_covers;
			int column_to_remove = neighbour.minimal_cover[column_to_remove_index];

			//pick a random row from the non-covering
			int column_to_add_index = rand() % neighbour.number_of_non_covering;
			int column_to_add = neighbour.non_covering_columns[column_to_add_index];

			//Swap cols in the solution
			neighbour.non_covering_columns[column_to_add_index] = column_to_remove;
			neighbour.minimal_cover[column_to_remove_index] = column_to_add;

			//update
			neighbour.columns_in_solution[column_to_add] = COVERED;
			neighbour.columns_in_solution[column_to_remove] = NOT_COVERED;
		}
		if (is_feasible(instance, &neighbour)) {
			solution_found = TRUE;
		}
		else {
			non_feasible++;
		}
	}

	return neighbour;
}
