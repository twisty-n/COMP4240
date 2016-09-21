#pragma once

#include "stdafx.h"
#include "solution.h"
#include "problem_instance.h"

/*
 Generates a solution to the given instance by randomly selecting the next uncovered row
 and adding a column that covers that row
 */
void random_construction(Instance * instance, Solution * solution);