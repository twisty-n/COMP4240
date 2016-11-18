#include "stdafx.h"
#include "utilities.h"
#include "constructive_heuristic.h"
#include "solution.h"
#include "problem_instance.h"
#include <stdlib.h>
#include "local_search.h"
#include <assert.h>
#include "simulated_annealing.h"

double rand_in_range(int min, int max)
{
	double range = (max - min);
	double div = RAND_MAX / range;
	return min + (rand() / div);
}

Solution * generate_population(Instance * instance, int population_size) {
	Solution * initial_population = (Solution *) malloc(sizeof(Solution) * population_size);
	for (int j = 0; j < population_size; j++) {
		Solution * s = (Solution *) malloc(sizeof(Solution));
		greedy_construction(instance, s, FALSE);
		int max_mutation_count = s->number_of_covers / 10.0;
		int current_mutation_count = 0;
		for (int i = 0; i < instance->column_count && current_mutation_count != max_mutation_count; i++) {
			if (s->columns_in_solution[i] != TRUE) {
				double mutate = rand_in_range(0,1);
				if (mutate < 0.1) {
					add_column(instance, s, i);
					current_mutation_count++;
				}
			}
		}
		initial_population[j] = *s;
	}

	return initial_population;
}

boolean terminate(int current_iteration, Solution * solutions) {
	return current_iteration > 10 ? TRUE : FALSE;
}

void make_feasible(Instance * instance, Solution * target) {
	// add columsn to cover things with the lowest cost ratio
	// ratio is defined as cost / number of uncovered rows that it covers

	int * rows = (int *) calloc(instance->row_count, sizeof(int));

	// Preprocess to see which rows are uncovered

	target->minimal_cover = reverse_quick_sort(target->minimal_cover, 0, instance->row_count - 1, instance->row_count);

	//print_array(target->minimal_cover, instance->row_count);
	//printf("Number of covers: %d", target->number_of_covers);
	//printf("\n");

	for (int i = 0; i < target->number_of_covers; i++) {
		int covering_column = target->minimal_cover[i];
		assert(covering_column != -1);
		for (int i = 0; i < instance->row_count; i++) {
			if (instance->matrix[i][covering_column] == TRUE) {
				rows[i] = TRUE;
			}
		}
	}
	while (!is_feasible(instance, target)) {
		for (int i = 0; i < instance->row_count; i++) {
			if (rows[i] != TRUE) {
				//printf("covering %d", i);
				// Find the best column to cover it
				long current_best_ratio = LONG_MAX;
				int best_column = -1;

				for (int j = 0; j < target->number_of_non_covering; j++) {
					int candidate = target->non_covering_columns[j];
					
					if (instance->matrix[i][candidate] != TRUE) continue;

					int cost = instance->column_costs[candidate];
					int covers = 0;
					for (int i = 0; i < instance->row_count; i++) {
						if (instance->matrix[i][candidate] == TRUE) {
							if (rows[i] == FALSE) {
								covers++;
							}
						}
					}
					if (covers != 0) {
						long ratio = ((long)cost) / ((long)covers);
						if (ratio < current_best_ratio) {
							current_best_ratio = ratio;
							best_column = candidate;
						}
					}
				}

				for (int i = 0; i < instance->row_count; i++) {
					if (instance->matrix[i][best_column] == TRUE) {
						rows[i] = TRUE;
					}
				}

				add_column(instance, target, best_column);
				break;
			}
		}
	}

	free(rows);
}

void merge(Instance * instance, Solution * target, Solution * source) {
	int number_columns_to_modify = (int)rand_in_range(1, target->number_of_covers / 3.0);
	for (int i = 0; i < number_columns_to_modify; i++) {
		boolean do_remove = rand_in_range(0, 1) < 0.5;
		int candidate = target->minimal_cover[((int)rand_in_range(0, target->number_of_covers - 1))];
		if (do_remove) {
			remove_column(instance, target, candidate);
		}
		else {
			add_column(instance, target, candidate);
		}
	}
	make_feasible(instance, target);
}

Solution jpso(Instance * instance, int population_size) {
	Solution * population = generate_population(instance, population_size);
	Solution * personal_bests = (Solution*)malloc(population_size * sizeof(Solution));
	
	for (int i = 0; i < population_size; i++) {
		personal_bests[i] = deep_copy(instance, &population[i]);
	}
	
	Solution current_best = deep_copy(instance, &population[0]);
	Solution global_best = deep_copy(instance, &population[0]);
	int current_iteration = 0;

	Solution * modifier;
	while (!terminate(current_iteration, population)) {
		for (int i = 0; i < population_size; i++) {
			double selector = rand_in_range(0, 1);
			if (selector <= 0.25) {
				modifier = &population[i];
			}
			else if (selector <= 0.50) {
				modifier = &personal_bests[i];
			}
			else if (selector <= 0.75) {
				modifier = &current_best;
			}
			else {
				modifier = &global_best;
			}
			
			// Merge, and them perform the local search
			merge(instance, &population[i], modifier);

			// Restore when we have a working local search
			//population[i] = *perform_simulated_annealing(instance, &population[i]);
			//population[i] = deep_copy(instance, test);
			//population[i] = deep_copy(instance, &population[i]);

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
		current_iteration++;
	}

	return global_best;
}