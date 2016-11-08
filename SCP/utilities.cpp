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


boolean check_feasibility(Instance * instance, Solution * solution) {
	for (int i = 0; i < instance->row_count; i++) {
		if (solution->covering_columns[i] == 0) {
			return FALSE;
		}
	}
	return TRUE;
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

Solution deep_copy(Instance * instance,Solution * solution_s0) {

	Solution deep_copy;
	deep_copy.cost = solution_s0->cost;
	deep_copy.number_of_covers = solution_s0->number_of_covers;
	deep_copy.number_of_non_covering = solution_s0->number_of_non_covering;
	deep_copy.time = solution_s0->time;

	deep_copy.covering_columns = copy_array(solution_s0->covering_columns, instance->row_count);
	deep_copy.minimal_cover = copy_array(solution_s0->minimal_cover, instance->row_count);
	deep_copy.columns_in_solution = copy_array(solution_s0->columns_in_solution, instance->column_count);
	deep_copy.non_covering_columns = copy_array(solution_s0->non_covering_columns, solution_s0->number_of_non_covering);
		
	return deep_copy;
}

int * copy_array(int * array, int size) {
	int * new_array = (int *)calloc(size, sizeof(int));;
	for (int i = 0; i < size; i++) {
		new_array[i] = array[i];
	}
	return new_array;
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