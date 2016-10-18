#pragma once

#include "stdafx.h"
#include "solution.h"
#include "problem_instance.h"

/*
 Generates a solution to the given instance by randomly selecting the next uncovered row
 and adding a column that covers that row
 */
void random_construction(Instance * instance, Solution * solution);

/*
Generates a solution to the given instance by greedily selecting the set (column) that
contains the largest number of uncovered elements.
*/
void greedy_construction(Instance * instance, Solution * solution);

/*
Used by greedy_construction to determine the next best row to add to the solution.  Will return the index of the
unassigned_colums array, which references the next best row to add to the sollution.
*/
void find_next_best_col(Instance * instance, int * best_col, int * uncovered_rows, int * no_uncovered_rows, int * unassigned_columns, int no_unassigned_columns, int * rows_to_be_covered_at_each_instance, int * no_current_rows);

/*
will be used to help remove a column from the set of columns not yet in the solution
*/
void remove_column(int * unassigned_columns, int * best_col, int * no_unassigned_columns);

/*
will be used to help remove a row from the set of row not yet covered by the solution
*/
void remove_rows(int * coverings, int * uncovered_rows, int * no_uncovered_rows, int * rows_to_be_covered_this_instance, int * no_current_rows, int * best_col);


/*
Used by greedy if an array needs to be sorted - lowest to highest
follows: http://www.cprogramming.com/tutorial/computersciencetheory/quicksort.html
*/
void quick_sort(int * array, int p, int r);

/*
Used by quick_sort
*/
int partition(int * array, int p, int r);