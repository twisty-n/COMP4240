#pragma once

#include "solution.h"
#include "problem_instance.h"
#include "constructive_heuristic.h"

/*
This is the interface which SCP should call when trying to implement a best_accept local search.
Runs a greedy construction on the solution to generate S0, then calls to local_search_1
*/
void perform_local_search_1(Instance * instance, Solution * solution);

/*
will find other solutions in the neighbourhood, evaluate fitness and accept using a hill climbing
best fit method.
*/
void local_search_1(Instance * instance, Solution * solution);

/*
This is the interface which SCP should call when trying to implement a first_accept local search.
Runs a random construction on the solution to generation S0, then calls local_search_first_accept.
*/
void perform_local_search_first_accept(Instance * instance, Solution * solution);

/*
will find other solutions in the neighbourhood, evaluate fitness and accept using a first accept method.
*/
void local_search_first_accept(Instance * instance, Solution * solution);