#include "stdafx.h"
#include <stdlib.h>
#include "solution.h"

void print_solution(Solution * solution) {
	printf("\n");
	printf("Total Cost: %d\n", solution->cost);
	printf("Required Covers %d\n", solution->number_of_covers);
	printf("Minimal Covering Columns: \n");
	for (int i = 1; i < solution->number_of_covers; i++) {
		// Plus 1 so that the columns are displayed sensibly
		printf("%5d ", solution->minimal_cover[i-1]+1);
		if (i % 6 == 0 && i != 0) {	
			// Prints 6 per row (arbitarily) to fit into the console window
			printf("\n");
		}
	}
	printf("\n");
}

void print_solution_stats(Solution * solution) {
	printf("%d ",solution->cost);		//prints basic info so python can add to the exec summary
	printf("%6.4lf",solution->time);
}

void print_solution_to_file(Solution * solution, FILE * file) {


}

void free_solution(Solution * solution) {
	free(solution->minimal_cover);
	free(solution->covering_columns);
}

