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


void greedy_construction(Instance * instance, Solution * solution) {
	
	boolean solution_covered = FALSE;

	// First create an array which will keep a reference to the covering columns - this is an array
	// which will keep track of rows covered by the solution.  array index matches the row, array[i]
	// represents the first column selected which covered it.
	// minimal_coverings will keep track of which columns are included in the solution
	// also need to keep track of the current cost and the number of coverings in the solution
	int * coverings = (int *)calloc(instance->row_count, sizeof(int));
	int * minimal_coverings = (int *)calloc(instance->row_count, sizeof(int));
	int current_cost = 0;
	int number_of_coverings = 0;

	/*************************************************************************************
	*  for greedy - items which are not yet in the soltuion are important to track
	*************************************************************************************/
	// create an array which will be used to define which rows have not been covered
	// by the solution and init with all rows uncovered.  The array will be managed by its
	// logical size
	int * uncovered_rows = (int *)calloc(instance->row_count, sizeof(int));
	int no_uncovered_rows = instance->row_count;
	for (int i = 0; i < no_uncovered_rows; i++) {
		uncovered_rows[i] = i;
	}


	// next, create an array which will be used to define the colums not presently
	// considered part of the solution also to be managed by its logical size
	int * unassigned_columns = (int *)calloc(instance->column_count, sizeof(int));
	int no_unassigned_columns = instance->column_count;
	for (int i = 0; i < no_unassigned_columns; i++) {
		unassigned_columns[i] = i;
	}


	//for each iteration we need to know
	int best_col;						//will hold a reference to the index of the column which is to be added to the solution	
	int no_current_rows;				//will be used to keep track of the number of rows (current NOT in the solution) which are about to be added
	int i;								//misc loop and index control
	int j;
	int n;

	// finally, we need an array which is going to keep track of which rows are being covered by each instance, i.e; when
	// a coloumn is added to the solution.  This way we can keep the uncovered_rows_array and the coverings array up to date.
	int * rows_to_be_covered_this_instance = (int *)calloc(instance->row_count, sizeof(int));


	//begin adding members greedily
	while ( !solution_covered ) {

		//find the best column to add to the solution, and will update the rows_to_be_covered_at_each_instance array.
		find_next_best_col(instance, &best_col, uncovered_rows, &no_uncovered_rows, unassigned_columns, no_unassigned_columns, rows_to_be_covered_this_instance, &no_current_rows);

		//add best col index to the solution
		//remove it from the set of columns not currently in the solution
		//and update the list of uncovered rows based on what the column just covered
		minimal_coverings[number_of_coverings] = best_col;
		number_of_coverings++;
		current_cost += instance->column_costs[best_col];
		remove_column(unassigned_columns, &best_col, &no_unassigned_columns);
		remove_rows(coverings, uncovered_rows, &no_uncovered_rows, rows_to_be_covered_this_instance, &no_current_rows, &best_col);

		//building the solution is complete when all rows are considered covered by the soltuion
		if (no_uncovered_rows == 0) {
			solution_covered = TRUE;
		}
	}
	
	//ensure minimal_coverings is sorted before passing to the solution
	//quick_sort(minimal_coverings, 1, number_of_coverings-1);

	//update solution details with results of the cover
	solution->cost = current_cost;
	solution->covering_columns = coverings;
	solution->minimal_cover = minimal_coverings;
	solution->number_of_covers = number_of_coverings;

	//TODO:  free memory from the callocs

}

void find_next_best_col(Instance * instance, int * best_col, int * uncovered_rows, int * no_uncovered_rows, int * unassigned_columns, int no_unassigned_columns, int * selected_rows_for_this_iteration, int * no_selected_rows) {

	int best_col_local_index = 0;		//holds a reference to the best column index from the local array un_assigned_columns
	int best_value = 0;					//will keep track of the best value a column has thus far
	int * best_rows_to_be_covered;		//will keep track of the rows being covered by the best solution
	int current_col_index;				//will hold a reference to the index (in the instance) of the column to be evalauted
	int current_row_matrix_index;		//will hold a reference to the index (in the instance) of the row to be evaluated
	int current_col_value;				//will hold the current value of the column being evaluated	TODO:  current will hold the no of rows, but may need to change when not unicost
	int no_current_rows;				//will hold the number of the rows being covered each column
	int * current_rows_to_be_covered;	//will keep track of the uncovered rows which are being covered by the coloumn being evaluated
	

	current_rows_to_be_covered	=	(int *)calloc(*no_uncovered_rows, sizeof(int));	
	best_rows_to_be_covered		=	(int *)calloc(*no_uncovered_rows, sizeof(int));
	
	// find the value-add of any columns not currently in the solution, and track the best one.  keep track of which
	// rows it will cover, but only based on the rows currently not listed in the solution
	
	// for each un_assigned column
	for (int i = 0; i < no_unassigned_columns; i++) {
		
		current_col_index = unassigned_columns[i];
		
		//init colum details to 0
		current_col_value = 0;
		no_current_rows = 0;


		//find how many rows (which are not yet covered by the solution) the column has a members
		for (int j = 0; j < *no_uncovered_rows; j++){
			current_row_matrix_index = uncovered_rows[j];			
			if (instance->matrix[current_row_matrix_index][current_col_index] > 0) {
				no_current_rows += 1;
				current_col_value += 1;														//TODO:  change the value calcuation
				current_rows_to_be_covered[no_current_rows-1] = current_row_matrix_index;
			}
		}

		if (current_col_value > best_value) {
			best_col_local_index = i;
			best_value = current_col_value;
			*no_selected_rows = no_current_rows;

			//get the details of the rows to be covered by the column.
			//TODO:  may need to change how this loop is controlled when the heuristic is no longer uni-cost
			for (int n = 0; n < *no_selected_rows; n++) {
				best_rows_to_be_covered[n] = current_rows_to_be_covered[n];
			}
		}
	}

	// at this point - you should know the best column to be covered as well as the rows which it will cover.
	// Update the rows_to_be_covered_at_each_instance array, and keep track of the best_col so that the rest of 
	// the heuristic can run
	*best_col = unassigned_columns[best_col_local_index];

	for (int i = 0; i < *no_selected_rows; i++) {
		selected_rows_for_this_iteration[i] = best_rows_to_be_covered[i];
	}
}

void remove_column(int * unassigned_columns, int * best_col, int * no_unassigned_columns) {
	for (int i = 0; i < *no_unassigned_columns; i++) {
		if (unassigned_columns[i] == *best_col) {
			for (int n = i; n < *no_unassigned_columns - 1; n++) {
				unassigned_columns[n] = unassigned_columns[n + 1];
			}
			*no_unassigned_columns -=1;
			break;
		}
	}
}


void remove_rows(int * coverings, int * uncovered_rows, int * no_uncovered_rows, int * rows_to_be_covered_this_instance, int * no_current_rows, int * best_col){

	//traverse the set of rows going into the solution, and remove them from the larger rows not yet in the solution
	//keep the larger array sorted as you go
	for (int i = 0, j = 0; i < * no_current_rows; i++) {

		//for each row to be covered, update its reference in coverings to define which col covered it
		coverings[rows_to_be_covered_this_instance[i]] = *best_col;

		//find the row to be removed from the uncovered rows list
		//if you dont have a match, traverse along the uncovered_rows list some more
		while (rows_to_be_covered_this_instance[i] != uncovered_rows[j]) {
			j += 1;
		}

		//when you get the match, remove it from the list of uncovered rows and adjust the array and logical size
		for (int n = j; n < *no_uncovered_rows - 1; n++) {
			uncovered_rows[n] = uncovered_rows[n + 1];
		}
		*no_uncovered_rows -=1;

		//push j along to the next element in the list
		j++;
	}
}






void quick_sort(int * array, int p, int r) {
	
	int q;
	
	if (p < r) {
		q = partition(array, p, r);
		return quick_sort(array, p, q - 1);
		return quick_sort(array, q + 1, r);
	}
}

int partition(int * array, int p, int r) {

	int i = p - 1;
	int temp;

	for (int j = p; j < r; j++) {
		if (array[j] <= array[r]) {
			i = i - 1;
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
		}
	}

	temp = array[i + 1];
	array[i + 1] = array[r];
	array[r] = temp;

	return i + 1;
}

