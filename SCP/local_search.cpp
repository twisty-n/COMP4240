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


Solution * local_search_first_accept(Instance * instance, Solution * solution_S0) {	

	Solution current = *solution_S0;
	Solution best_neighbour;
	boolean terminate_search = FALSE;
	Solution neighbourhood[NEIGHBOURHOOD_SIZE];

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
			neighbour.cost -= instance->column_costs[column_to_remove];
			neighbour.cost += instance->column_costs[column_to_add];

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


Solution not_so_randomly_generate_neighbour(Instance * instance, Solution * solution_S0) {

	int total_cols_to_remove = 5;
	int number_of_rows_losing_cover;
	int * rows_losing_cover;
	int column_to_remove;
	
	Solution neighbour = deep_copy(instance, solution_S0);

	//find the rows that have the highest coverage in the solution
	int * rows_with_most_coverage = (int *)calloc(total_cols_to_remove, sizeof(int));
	find_rows_with_most_coverage(instance, &neighbour, rows_with_most_coverage, &total_cols_to_remove);


	//for each of these rows, remove a selected column which is covering one of these rows from the solution
	for (int i = 0; i < total_cols_to_remove; i++){
	
		//find a column to remove
		column_to_remove = neighbour.covering_column[rows_with_most_coverage[i]];

		//find out how many rows will lose 1 cover because of this removal													
		number_of_rows_losing_cover = 0;
		int * rows_losing_cover = (int *)calloc(instance->row_count, sizeof(int));
		find_rows_covered_by_column(instance, &neighbour, rows_losing_cover, &column_to_remove, &number_of_rows_losing_cover);
	
		//remove the column from the solution
		remove_column(instance, &neighbour, column_to_remove);

		/******************************************************************************************************************
			Now it's all just housekeeping
		******************************************************************************************************************/
		//update details in the solution for each of the rows losing cover
		for (int j = 0; j < number_of_rows_losing_cover; j++) {	
			
			//decrement the number of covers for this row
			neighbour.covering_details.number_of_covers[rows_losing_cover[j]] -= 1;

			//if the column being removed was the representative col as covering this row, update it to something else.
			if (neighbour.covering_column[rows_losing_cover[j]] == column_to_remove) {
				//remove it from being the covering col
				neighbour.covering_column[rows_losing_cover[j]] = -1;
				
				//iterate through the cols listed as covering this row
				//if possible, find one that is already in the solution which can be listed as covering this row
				for (int k = 0; k < instance->row_covering_count[rows_losing_cover[j]]; k++) {
					if (neighbour.columns_in_solution[instance->raw_coverings[rows_losing_cover[j]][k]] == TRUE) {
						neighbour.covering_column[rows_losing_cover[j]] = instance->raw_coverings[rows_losing_cover[j]][k];
						break;
					}
				}
			}
		}
		free(rows_losing_cover);
	}
	free(rows_with_most_coverage);

	//check if any rows are missing cover and require a column to be added
	for (int row = 0; row < instance->row_count; row++) {
		
		if (neighbour.covering_details.number_of_covers[row] <= 0) {
			
			//get a list of columns which can cover the row
			int coverings_for_row = instance->row_covering_count[row];
			int * column_covers_for_row = instance->raw_coverings[row];

			//find the cheapest
			int min_cost = INT_MAX;
			int selected_col;
			for (int j = 0; j < coverings_for_row; j++) {
				if (instance->column_costs[column_covers_for_row[j]] < min_cost) {
					min_cost = instance->column_costs[column_covers_for_row[j]];
					selected_col = column_covers_for_row[j];
				}
			}

			//add the column to the solution then list it as the covering column for this row
			add_column(instance, &neighbour, selected_col);
			neighbour.covering_column[row] = selected_col;

			//find all other rows which are being covered because of this addition
			int number_of_rows_being_added = 0;
			int * rows_getting_cover = (int *)calloc(instance->row_count, sizeof(int));
			find_rows_covered_by_column(instance, &neighbour, rows_getting_cover, &selected_col, &number_of_rows_being_added);

			//and update their coverage details
			for (int k = 0; k < number_of_rows_being_added; k++) {
				if (neighbour.covering_details.number_of_covers[rows_getting_cover[k]] >= 0) {
					neighbour.covering_details.number_of_covers[rows_getting_cover[k]] += 1;
				}
				else {
					neighbour.covering_details.number_of_covers[rows_getting_cover[k]] = 1;
					neighbour.covering_column[rows_getting_cover[k]] = selected_col;
				}
			}
			free(rows_getting_cover);
		}
	}
	return neighbour;
}


void find_rows_with_most_coverage(Instance * instance, Solution * solution, int * rows_with_most_coverage, int * total_rows_to_remove) {

	//make copies of the row coverings and sort them.  Sort will be sorted from least cover to most cover
	int * row_index = copy_array(solution->covering_details.row_index, instance->row_count);
	int * covers_per_row = copy_array(solution->covering_details.number_of_covers, instance->row_count);
	quick_sort(row_index, covers_per_row, 0, instance->row_count - 1);

	//print to test
	//test_arrays_and_quick_sort(row_index, covers_per_row, 0, instance->row_count-1);

	//find the index's for the rows to remove this round
	for (int i = 0, j = instance->row_count - 1; i < *total_rows_to_remove; i++, j--) {
		rows_with_most_coverage[i] = row_index[j];
	}
	
	//print to test
	//print_array(rows_with_most_coverage, *total_rows_to_remove);
}


/*
	Finds the rows which are covered by a column.  pass in a pointer to a logical size int.  Pass it in as 0.
	The function will update this, so that by the end, it denotes the number of rows covered by a particular col.
*/
void find_rows_covered_by_column(Instance * instance, Solution * solution, int * rows_covered_by_column, int * selected_col, int * logical_size) {
	
	//find which rows are being covered by this column
	for (int row = 0; row < instance->row_count; row++) {
		if (instance->matrix[row][*selected_col] == 1) {
			rows_covered_by_column[*logical_size] = row;
			*logical_size += 1;
			rows_covered_by_column = expand_array(rows_covered_by_column, *logical_size);
		}
	}
	//for testing
	//print_array(rows_covered_by_column, *logical_size);
}


/*
	Finds the rows which are covered by a column.  pass in a pointer to a logical size int.  Pass it in as 0.
	The function will update this, so that by the end, it denotes the number of rows covered by a particular col.
*/
int * find_rows_covered_by_column(Instance * instance, Solution * solution, int * selected_col, int * logical_size) {
	
	int * rows_covered = (int *)calloc(instance->row_count, sizeof(int));

	//find which rows are being covered by this column
	for (int row = 0; row < instance->row_count; row++) {
		if (instance->matrix[row][*selected_col] == 1) {
			rows_covered[*logical_size] = row;
			*logical_size += 1;
		}
	}
	//for testing
	//print_array(rows_covered_by_column, *logical_size);
	return rows_covered;
}