#include "stdafx.h"
#include "simulated_annealing.h"
#include "local_search.h"
#include "constructive_heuristic.h"
#include "utilities.h"
#include <time.h>

/*
	And thus begins the simulated annealing
*/
Solution * perform_simulated_annealing(Instance * instance, Solution * solution) {

	//randomly contruct a solution
	random_construction(instance, solution);

	//define the number of times you will run the solution.





}


/*
	will find other solutions in the neighbourhood, evaluate fitness and accept using a hill climbing
	best fit method.
*/
int probaility(Solution * Scurrent, Solution * Snew, double T) {



}