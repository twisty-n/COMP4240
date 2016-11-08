#pragma once

#include "stdafx.h"

#define TRUE 1
#define FALSE 0

typedef int boolean;


/*
If an array needs to be sorted - lowest to highest
follows: http://www.algolist.net/Algorithms/Sorting/Quicksort
*/
void quick_sort(int * array, int p, int r);
