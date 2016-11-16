#include "stdafx.h"
#include "utilities.h"
#include <stdlib.h>
#include <time.h>


//TODO:  has an off-by-one error.  It is not sorting the element at idex zero correctly.
void quick_sort(int * arr, int left, int right) {
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quick_sort(arr, left, j);
	if (i < right)
		quick_sort(arr, i, right);
}

int compare(Solution * solution_a, Solution * solution_b) {
	if (solution_a->cost == solution_b->cost) {
		return 0;
	}
	else if (solution_a->cost < solution_b->cost) {
		return -1;
	}
	else {
		return 1;
	}
}

Solution deep_copy(Instance * instance, Solution * solution_s0) {

	Solution deep_copy;
	deep_copy.cost = solution_s0->cost;
	deep_copy.number_of_covers = solution_s0->number_of_covers;
	deep_copy.number_of_non_covering = solution_s0->number_of_non_covering;
	deep_copy.time = solution_s0->time;
	deep_copy.covering_column = copy_array(solution_s0->covering_column, instance->row_count);
	deep_copy.minimal_cover = copy_array(solution_s0->minimal_cover, instance->row_count);
	deep_copy.columns_in_solution = copy_array(solution_s0->columns_in_solution, instance->column_count);
	deep_copy.non_covering_columns = copy_array(solution_s0->non_covering_columns, solution_s0->number_of_non_covering);
	try {
		deep_copy.number_of_columns_covering_rows = copy_array(solution_s0->number_of_columns_covering_rows, instance->row_count);
	} catch ( ... ) {
		//do nothing
	}

	return deep_copy;
}

int * copy_array(int * array, int size) {
	int * new_array = (int *)calloc(size, sizeof(int));;
	for (int i = 0; i < size; i++) {
		new_array[i] = array[i];
	}
	return new_array;
}


#define NOT_COVERED 0
#define COVERED 1
boolean is_feasible(Instance * instance, Solution * solution) {

	// Array for all the rows that need to be covered, assume all uncovered
	int * rows_in_instance = (int *)calloc(instance->row_count, sizeof(int));
	int number_of_columns = instance->column_count;

	// For each column in the solution, see which rows it covers
	for (int i = 0; i < number_of_columns; i++) {
		// Inspect each one to see if it is in the solution
		if (solution->columns_in_solution[i] == TRUE) {
			// Mark each row that this guy covers as covered
			int column = i;
			for (int i = 0; i < instance->row_count; i++) {
				if (instance->matrix[i][column] == COVERED) {
					rows_in_instance[i] = COVERED;
				}
			}
		}
	}

	// Once this is done, iterate over the rows_in_instance and see if any are 0
	for (int i = 0; i < instance->row_count; i++) {
		if (rows_in_instance[i] == NOT_COVERED) {
			free(rows_in_instance);
			return FALSE;
		}
	}

	// Otherwise everthing is all good
	free(rows_in_instance);
	return TRUE;
}

void set_to_minus_ones(int * array, int size) {
	for (int i = 0; i < size; i++) {
		array[i] = -1;
	}
}


void print_array(int * array, int size) {
	for (int i = 0; i < size; i++) {
		// Plus 1 so that the columns are displayed sensibly
		printf("%5d ", array[i]);
		if (i % 6 == 0 && i != 0) {
			// Prints 6 per row (arbitarily) to fit into the console window
			printf("\n");
		}
	}
}

void print_array_real_column_labels(int * array, int size) {
	for (int i = 1; i <= size; i++) {
		// Plus 1 so that the columns are displayed sensibly
		printf("%5d ", array[i - 1] + 1);
		if (i % 6 == 0 && i != 0) {
			// Prints 6 per row (arbitarily) to fit into the console window
			printf("\n");
		}
	}
}

time_t get_current_time() {
	// Get the current time
	time_t now = time(0);
	struct tm time_val;
	localtime_s(&time_val, &now);

	char time_string[9];
	strftime(time_string, sizeof(time_string), "%H:%M:%S", &time_val);

	//printf("%s", time_string);

	return now;
}


double bounded_rand() {
	return ((double)rand() / (RAND_MAX));
}
