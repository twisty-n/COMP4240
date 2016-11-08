#pragma once

#include "stdafx.h"
#include "solution.h"
#include "problem_instance.h"

typedef int boolean;

/*
 Generates a solution to the given instance by randomly selecting the next uncovered row
 and adding a column that covers that row
 */
void random_construction(Instance * instance, Solution * solution);

/*
Generates a solution to the given instance by greedily selecting the set (column) that
contains the largest number of uncovered elements.
uni_cost mode:	TRUE - the solution generated picks columns at each iteration which
				contribute the most rows to the solution, regardless of cost
				FALSE - the solution genrated picks columns at each iteration, taking
				into account the column cost.
*/
void greedy_construction(Instance * instance, Solution * solution, boolean uni_cost);

/*
Used by greedy_construction to determine the next best row to add to the solution.  Will return the index of the
unassigned_colums array, which references the next best row to add to the sollution.
*/
void find_next_best_col(Instance * instance, int * best_col, int * uncovered_rows, int * no_uncovered_rows, int * unassigned_columns, int no_unassigned_columns, int * rows_to_be_covered_at_each_instance, int * no_current_rows, boolean uni_cost);

/*
will be used to help remove a column from the set of columns not yet in the solution
*/
void remove_column(int * unassigned_columns, int * best_col, int * no_unassigned_columns);

/*
will be used to help remove a row from the set of row not yet covered by the solution
*/
void remove_rows(int * coverings, int * uncovered_rows, int * no_uncovered_rows, int * rows_to_be_covered_this_instance, int * no_current_rows, int * best_col);

/*
will be used to help work out the best column in cases of tie break.  Currently the column covering rows which have the lowest probability of being
covered by other columns not yet in the solution will be selected.
*/
int tie_break(Instance * instance, int * current_col_index, int * current_rows_to_be_covered, int * no_current_rows, int * best_col_local_index, int * best_rows_to_be_covered, int * no_best_rows, int * no_unassigned_columns, int * unassigned_columns);
