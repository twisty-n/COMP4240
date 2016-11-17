#pragma once

#include "stdafx.h"
#include "solution.h"
#include "problem_instance.h"

/*
	will find other solutions in the neighbourhood, evaluate fitness and accept using a hill climbing
	best fit method.
*/
Solution * local_search_best_accept(Instance * instance, Solution * solution);

/*
	will find other solutions in the neighbourhood, evaluate fitness and accept using a first accept method.
*/
Solution * local_search_first_accept(Instance * instance, Solution * solution);

/*
	will randomly generate neighbours of a solution s0, by randomly swapping x columns {x: 0 < x < total_rows_to_swap} from
	within the solution with a column outside of the solution.  If no feasible solution is generated within 
	{x: 0 <= x < non_feasibile_max attempts} a the original solution will be returned.
*/
Solution randomly_generate_neighbour(Instance * instance, Solution * solution_S0);

/*
	will be used to generate neighbours by pruning the solution.
	
	processes:  Identify rows with the most coverage.
	
	Randomly select one of the columns covering for removal.

	if (removal of the colum removes a row that is only covered once)
		Find a column from the non-covering cols which can be sub'd in.
		If no substitute row exists
		break
    else
		Remove Column

*/

Solution not_so_randomly_generate_neighbour(Instance * instance, Solution * solution_S0);

/*
	Helps identify rows from the solution which had the most coverage

	populates an array with 
*/
void find_rows_with_most_coverage(Instance * instance, Solution * solutionnS0, int * rows_with_most_coverage, int * total_rows_to_remove);

/*
	will scan the matrix to work out how many rows are losing cover due to removal of a colum
	Will returns an array with the id's of rows that are covered by a column
	Will keeps the value of number_of_rows_losing_cover updated to represent how rows a colum covered
	(when calling, provide 0 as the number_of_rows_losing_cover).
*/
int * find_rows_losing_cover(Instance * instance, Solution * solution, int * column_to_remove, int * number_of_rows_losing_cover);