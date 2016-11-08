#pragma once

#include "stdafx.h"
#include "problem_instance.h"
#include "solution.h"

#define TRUE 1
#define FALSE 0

typedef int boolean;


/*
	If an array needs to be sorted - lowest to highest
	follows: http://www.algolist.net/Algorithms/Sorting/Quicksort
*/
void quick_sort(int * array, int p, int r);


/*
	will return ture is all rows are covered by a column
*/
boolean check_feasibility(Instance * instance, Solution * solution);
