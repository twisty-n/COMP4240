#pragma once

#include "stdafx.h"
#include "solution.h"
#include "problem_instance.h"


#define T_INIT  
#define T_FINAL 0.0001
#define ALPHA


/*
	And thus begins the simulated annealing
*/
Solution * perform_simulated_annealing(Instance * instance, Solution * solution);

/*
	will find other solutions in the neighbourhood, evaluate fitness and accept using a hill climbing
	best fit method.
*/
int probaility(Solution * Scurrent, Solution * Snew, double T);