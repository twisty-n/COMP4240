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

	//Init Environment:
	Solution current_sol;
	Solution neighbour;
	random_construction(instance, solution);
	current_sol = *solution;
	neighbour = deep_copy(instance, solution);


	//define the number of times you will run the solution.
	int k = 100;
	int k_final = 1;
	double T;

	while (k > k_final) {

		//generate the temperature (gently cools the algorithm)
		T = double(k) / double(k_final);
		
		//generate a neighbour
		neighbour = randomly_generate_neighbour(instance, &neighbour);

		//decide if you will keep the neighbour
		if (probaility(&current_sol, &neighbour, T) >= ) {
				
		}
		k++;
	}


	return solution;
}


/*
	will find other solutions in the neighbourhood, evaluate fitness and accept using a hill climbing
	best fit method.
*/
double probaility(Solution * Scurrent, Solution * Snew, double T) {


	return 0;
}