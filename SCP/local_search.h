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
	will be used to generate neighbours in a more strategic way.
	strategy to be defined.
*/

Solution not_so_randomly_generate_neighbour(Instance * instance, Solution * solution_S0);