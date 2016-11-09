#pragma once

#include "stdafx.h"
#include "solution.h"
#include "problem_instance.h"


/*
	And thus begins the simulated annealing
*/
Solution * perform_simulated_annealing(Instance * instance, Solution * solution);

/*
	Will perform the probability funciton on 2 states:
	Note: The probability function must return a positive value even when e' is greater than e.
	This feature prevents the method from becoming stuck at a local minimum that is worse than the global one.

	Engery of the state is the evalatuion of the cost of the state.

	The formulation of the probably is the method by Kirkpatrick et al. the acceptance probability
	function P(e,e',T) was defined as 1 if e' < e, and exp(-(e'-e)/T) otherwise. 

*/
double probaility(Solution * Scurrent, Solution * Snew, double T);