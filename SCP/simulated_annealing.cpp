#include "stdafx.h"
#include "simulated_annealing.h"
#include "local_search.h"
#include "constructive_heuristic.h"
#include "utilities.h"
#include <time.h>
#include "math.h"

/*
	And thus begins the simulated annealing
*/
Solution * perform_simulated_annealing(Instance * instance, Solution * solution) {

	//Init solutions to work with
	Solution current_sol;
	Solution neighbour;
	random_construction(instance, solution);

	//greedy_construction(instance, solution, FALSE);
	
	current_sol = deep_copy(instance, solution);
	neighbour = deep_copy(instance, solution);
	int solution_not_found = 0;					//will keep track of the number of times an agreeable solution is not found.
	int solution_not_found_max = 50;			//allows termination of algorithim if an agreeable soltion is not really found.


	//define the number of times you will run the solution.
	int k = 1;
	int k_final = 100;
	double T;

	while (k < k_final && solution_not_found < solution_not_found_max) {

		//generate the temperature (Used in probaility(), acts to gently cool the algorithm)
		T = double(k) / double(k_final);
		
		//generate a neighbour
		neighbour = randomly_generate_neighbour(instance, &neighbour);

		//decide if you will keep the neighbour
		if (probaility(&current_sol, &neighbour, T) >= bounded_rand()) {
			free_solution(&current_sol);
			current_sol = neighbour;
			solution_not_found = 0;
		}
		else {
			solution_not_found++;
		}
		k++;
	}
	return &current_sol;
}


double probaility(Solution * Scurrent, Solution * Snew, double T) {

	if (compare(Snew, Scurrent) == -1) {
		return 1.0;
	}
	else {
		return exp(-(Snew->cost - Scurrent->cost) / T);
	}
}