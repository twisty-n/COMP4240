#pragma once

struct Solution {
	int cost;
	int * covering_columns;
	int * minimal_cover;
	int number_of_covers; 
	int * non_covering_columns;		//greedy heuristic will update solution with the non_covering_columns
	int number_of_non_covering;		
	double time;
};

typedef struct Solution Solution;

void print_solution(Solution * solution);

void print_solution_stats(Solution * solution);

void print_solution_stats(Solution * solution, double average_cost, double average_time);

void free_solution(Solution * solution, FILE * file);

void free_solution(Solution * solution);

void print_solution_to_file(Solution * solution, FILE * file,  char * heuristic);