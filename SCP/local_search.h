#pragma once

#include "stdafx.h"
#include "solution.h"
#include "problem_instance.h"

/*
	This is the interface which SCP should call when trying to implement a best_accept local search.
	Runs a greedy construction on the solution to generate S0, then calls to local_search_1
*/
void perform_local_search_best_accept(Instance * instance, Solution * solution);

/*
	will find other solutions in the neighbourhood, evaluate fitness and accept using a hill climbing
	best fit method.
*/
void local_search_best_accept(Instance * instance, Solution * solution);

/*
	This is the interface which SCP should call when trying to implement a first_accept local search.
	Runs a random construction on the solution to generation S0, then calls local_search_first_accept.
*/
void perform_local_search_first_accept(Instance * instance, Solution * solution, time_t start_sol);

/*
	will find other solutions in the neighbourhood, evaluate fitness and accept using a first accept method.
*/
void local_search_first_accept(Instance * instance, Solution * solution);

/*
	will randomly generate neighbours of a solution s0, by randomly swapping x columns {x: 0 < x < total_rows_to_swap} from
	within the solution with a column outside of the solution.  If no feasible solution is generated within 
	{x: 0 <= x < non_feasibile_max attempts} a the original solution will be returned.
*/
Solution randomly_generate_neighbour(Instance * instance, Solution * solution_S0);