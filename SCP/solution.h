#pragma once

struct Solution {
	int cost;
	int * covering_columns;
	int * minimal_cover;
	int number_of_covers;
	double time;
};

typedef struct Solution Solution;

void print_solution(Solution * solution);

void print_solution_stats(Solution * solution);

void free_solution(Solution * solution, FILE * file);

void print_solution_to_file(Solution * solution, FILE * file, char * heuristic);