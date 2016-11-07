#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include "constructive_heuristic.h"
#include "utilities.h"

#define TRUE 1
#define FALSE 0

typedef int boolean;

void random_construction(Instance * instance, Solution * solution) {

	// First we create an array for the covering columns
	// This will be at most the number of rows, most likely less
	// The ith row will be covered by the value
	int * coverings = (int *) calloc(instance->row_count, sizeof(int));
	int * minimal_coverings = (int *) calloc(instance->row_count, sizeof(int));
	int * columns_in_solution = (int *)calloc(instance->column_count, sizeof(int));

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

		selected_column = column_covers_for_row[rand() % coverings_for_row];
		columns_in_solution[selected_column] = 1; // Flip the bit
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
	solution->columns_in_solution = columns_in_solution;
}


void greedy_construction(Instance * instance, Solution * solution, boolean uni_cost) {
	
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
	int no_current_rows;				//will be used to keep track of the number of rows from each iteration, not yet in the solution, but which are about to be added
	int i;								//misc loop and index control
	int j;
	int n;

	// finally, we need an array which is going to keep track of which rows are being covered by each instance, i.e; when
	// a coloumn is added to the solution.  This way we can keep the uncovered_rows_array and the coverings array up to date.
	int * rows_to_be_covered_this_instance = (int *)calloc(instance->row_count, sizeof(int));


	//begin adding members greedily
	while ( !solution_covered ) {

		//find the best column to add to the solution, and will update the rows_to_be_covered_at_each_instance array.
		find_next_best_col(instance, &best_col, uncovered_rows, &no_uncovered_rows, unassigned_columns, no_unassigned_columns, rows_to_be_covered_this_instance, &no_current_rows, uni_cost);

		//fprintf(debug_log, "\n\nCOL:  %d, COST:  %d,  COST THIS INSTANCE, %5.2f\n", best_col, instance->column_costs[best_col], (instance->column_costs[best_col] / (double)no_current_rows) );
		//fprintf(debug_log, "covering %d rows:  ", no_current_rows);
		//for (i = 0; i < no_current_rows; i++) {
		//	fprintf(debug_log, "%d ", rows_to_be_covered_this_instance[i]);
		//}
		
		//add best col index to the solution
		//remove it from the set of columns not currently in the solution
		//and update the list of uncovered rows based on what the column just covered
		minimal_coverings[number_of_coverings] = best_col;
		coverings[best_col] = 1;
		number_of_coverings++;
		current_cost += instance->column_costs[best_col];
		remove_column(unassigned_columns, &best_col, &no_unassigned_columns);
		remove_rows(coverings, uncovered_rows, &no_uncovered_rows, rows_to_be_covered_this_instance, &no_current_rows, &best_col);

		//fprintf(debug_log, "\nuncovered rows: ");
		//for (i = 0; i < no_uncovered_rows; i++) {
		//	fprintf(debug_log, "%d ", uncovered_rows[i]);
		//}
		//fprintf(debug_log, "\nend iter\n");

		//building the solution is complete when all rows are considered covered by the soltuion
		if (no_uncovered_rows == 0) {
			solution_covered = TRUE;
		}
	}
	
	//ensure minimal_coverings is sorted before passing to the solution
	quick_sort(minimal_coverings, 1, number_of_coverings-1);

	//tidy up the list of non-coveringing columns so you can pass it through to the solution
	int * non_covering_columns = (int *)calloc(no_unassigned_columns, sizeof(int));
	for (int i = 0; i < no_unassigned_columns; i++) {
		non_covering_columns[i] = unassigned_columns[i];
	}


	//update solution details with results of the cover
	solution->cost = current_cost;
	solution->covering_columns = coverings;
	solution->minimal_cover = minimal_coverings;
	solution->number_of_covers = number_of_coverings;
	solution->non_covering_columns = non_covering_columns;
	solution->number_of_non_covering = no_unassigned_columns;

	//free memory
	//free(uncovered_rows);
	//free(unassigned_columns);
	//free(rows_to_be_covered_this_instance);

}

void find_next_best_col(Instance * instance, int * best_col, int * uncovered_rows, int * no_uncovered_rows, int * unassigned_columns, int no_unassigned_columns, int * selected_rows_for_this_iteration, int * no_selected_rows, boolean uni_cost) {

	int best_col_local_index = 0;		//holds a reference to the best column index from the local array un_assigned_columns
	double best_value;					//will keep track of the best value a column has thus far - init to max
	int * best_rows_to_be_covered;		//will keep track of the rows being covered by the best solution
	int current_col_index;				//will hold a reference to the index (in the instance) of the column to be evalauted
	int current_row_matrix_index;		//will hold a reference to the index (in the instance) of the row to be evaluated
	double current_col_value;			//will hold the current value of the column being evaluated	TODO:  current will hold the no of rows, but may need to change when not unicost
	int no_current_rows;				//will hold the number of the rows being covered each column
	int best_currrent_rows;				//will hold the number of the best rows being covered in this iteration
	int * current_rows_to_be_covered;	//will keep track of the uncovered rows which are being covered by the coloumn being evaluated
	

	current_rows_to_be_covered	=	(int *)calloc(*no_uncovered_rows, sizeof(int));	
	best_rows_to_be_covered		=	(int *)calloc(*no_uncovered_rows, sizeof(int));
	if (uni_cost) {
		best_value = DBL_MIN;
	}
	else {
		best_value = DBL_MAX;
	}
	
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
				current_rows_to_be_covered[no_current_rows] = current_row_matrix_index;
				no_current_rows += 1;
			}
		}

		//work out what this column is contributing to the solution in its current state:
		if(no_current_rows > 0){  //avoids divide by zero for non-unicost

			if (uni_cost == TRUE) {
				current_col_value = no_current_rows;
				//now define if this is the best column you've seen so far.		
				if (current_col_value > best_value) {

					//if (current_col_value == best_value) {
					//	if (tie_break(instance, &current_col_index, current_rows_to_be_covered, &no_current_rows, &best_col_local_index, best_rows_to_be_covered, &*no_selected_rows, &no_unassigned_columns, unassigned_columns) == best_col_local_index)
					//		continue;
					//}
					best_col_local_index = i;
					best_value = current_col_value;
					best_currrent_rows = no_current_rows;

					//get the details of the rows to be covered by the column.
					for (int n = 0; n < best_currrent_rows; n++) {
						best_rows_to_be_covered[n] = current_rows_to_be_covered[n];
					}
				}
			}
			else 
			{
				current_col_value = instance->column_costs[current_col_index] / (double)no_current_rows;

				if (current_col_value < best_value) {

					//if (current_col_value == best_value) {
					//	if (tie_break(instance, &current_col_index, current_rows_to_be_covered, &no_current_rows, &best_col_local_index, best_rows_to_be_covered, &*no_selected_rows, &no_unassigned_columns, unassigned_columns) == best_col_local_index)
					//		continue;
					//}

					best_col_local_index = i;
					best_value = current_col_value;
					best_currrent_rows = no_current_rows;

					//get the details of the rows to be covered by the column.
					for (int n = 0; n < best_currrent_rows; n++) {
						best_rows_to_be_covered[n] = current_rows_to_be_covered[n];
					}
				}
			}
		}
	}//end checking unassigned columns

	// at this point - you should know the best column to be covered as well as the rows which it will cover.
	// Update the rows_to_be_covered_at_each_instance array, and keep track of the best_col so that the rest of 
	// the heuristic can run
	*best_col = unassigned_columns[best_col_local_index];
	*no_selected_rows = best_currrent_rows;

	for (int i = 0; i < *no_selected_rows; i++) {
		selected_rows_for_this_iteration[i] = best_rows_to_be_covered[i];
	}

	free(current_rows_to_be_covered);
	free(best_rows_to_be_covered);
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
	int rows_to_be_removed = *no_current_rows;

	for (int i = 0, j = 0; i < rows_to_be_removed; i++) {

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
	}
}

int tie_break(Instance * instance, int * current_col_index, int * current_rows_to_be_covered, int * no_current_rows, int * best_col_local_index, int * best_rows_to_be_covered, int * no_best_rows, int * no_unassigned_columns, int * unassigned_columns) {

	int best_tally = 0;			//keeps a tally of how many rows in the current (best) solution are covered by another colum.  +1 for each covering.
	int current_tally = 0;		//keeps a tally of how many rows in the current (proposed) solution are coverd by another column.  +1 for each covering.
	int current_col_ref;		//holds a reference to the column being tested
	int current_row_ref;		//holds a reference to the row being tested

	for (int i = 0; i < * no_unassigned_columns; i++) {
		current_col_ref = unassigned_columns[i];
		for (int j = 0; j < *no_best_rows; j++) {
			if (i == *best_col_local_index) {
				continue;			//skip if you hit the current column, you are only iterested in the other guys.
			}
			else {
				current_row_ref = best_rows_to_be_covered[j];
				if (instance->matrix[current_row_ref][current_col_ref] > 0) {
					++best_tally;
				}
			}
		}//end searching best rows
		for (int j = 0; j < *no_current_rows; j++) {
			if (i == *current_col_index) {
				continue;			//skip if you hit the current column, you are only iterested in the other guys.
			}
			else {
				current_row_ref = current_rows_to_be_covered[j];
				if (instance->matrix[current_row_ref][current_col_ref] > 0) {
					++current_tally;
				}
			}
		}//end seraching current (proposed) rows

	}//end searching all unassigned columns

	if (best_tally <= current_tally) {
		return *best_col_local_index;
	}
	else {
		return *current_col_index;
	}
}
