#include "utilities.h"
#include "constructive_heuristic.h"
#include "solution.h"
#include "problem_instance.h"
#include <stdlib.h>
#include "local_search.h"

Solution * generate_population(Instance * instance, int population_size) {
	Solution * initial_population = (Solution *) malloc(sizeof(Solution) * population_size);
	for (int i = 0; i < population_size; i++) {
		Solution * s = (Solution *) malloc(sizeof(Solution));
		greedy_construction(instance, s, FALSE);
		for (int i = 0; i < instance->column_count; i++) {
			if (s->columns_in_solution[i] != TRUE) {
				if (rand() < 0.5) {
					s->columns_in_solution[i] = TRUE;
				}
			}
		}
		initial_population[i] = *s;
	}
}

boolean terminate(int current_iteration, Solution * solutions) {
	return current_iteration > 20 ? TRUE : FALSE;
}

void make_feasible(Instance * instance, Solution * target) {

}

void merge(Instance * instance, Solution * target, Solution * source) {
	int number_columns_to_modify = rand() * ((2 * target->number_of_covers) / 3.0);
	for (int i = 0; i < number_columns_to_modify; i++) {
		boolean do_remove = rand() < 0.5;
		if (do_remove) {
			int candidate = target->minimal_cover[rand() * target->number_of_covers];
			target->columns_in_solution[candidate] = FALSE;
			// TODO housekeeping
		}
		else {
			int candidate = source->minimal_cover[rand() * source->number_of_covers];
			target->columns_in_solution[candidate] = TRUE;
			// TODO hosuekeeping
		}
	}
	make_feasible(instance, target);
}

Solution * jpso(Instance * instance, int population_size) {
	Solution * population = generate_population(instance, population_size);
	Solution * personal_bests;
	
	for (int i = 0; i < population_size; i++) {
		personal_bests[i] = deep_copy(instance, &population[i]);
	}
	
	Solution current_best = deep_copy(instance, &population[0]);
	Solution global_best = deep_copy(instance, &population[0]);
	int current_iteration = 0;

	Solution * modifier;

	while (!terminate(current_iteration, population)) {
		for (int i = 0; i < population_size; i++) {
			double selector = rand();
			if (selector < 0.25) {
				modifier = &population[i];
			}
			else if (selector < 0.5) {
				modifier = &personal_bests[i];
			}
			else if (selector < 0.75) {
				modifier = &current_best;
			}
			else {
				modifier = &global_best;
			}
			
			// Merge, and them perform the local search
			merge(instance, &population[i], modifier);
			population[i] = deep_copy(instance, local_search_best_accept(instance, &population[i]));

			if (population[i].cost < personal_bests[i].cost) {
				personal_bests[i] = deep_copy(instance, &population[i]);
			}
			if (population[i].cost < current_best.cost) {
				current_best = deep_copy(instance, &population[i]);
			}
			if (population[i].cost < global_best.cost) {
				global_best = deep_copy(instance, &population[i]);
			}
		}
	}
}