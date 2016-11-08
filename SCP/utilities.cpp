#include "stdafx.h"
#include "utilities.h"


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
	//will return TRUE if all rows in the solution it is checking are covered

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