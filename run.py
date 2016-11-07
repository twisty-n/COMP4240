import subprocess
import threading
import os
import csv
import sys
import numpy
import string
import xlsxwriter
from xlsxwriter.utility import xl_rowcol_to_cell  # used to generate A1 notations of cells
import time

# note for development: this xlsxwriter library will be highlighted in red because pycharm cannot find library
# but it is probably installed.  You can check if it is listed using 'pip list'.  Can install with pip as well,
# even if it is highlighted in red, it should work fine (from CMD)
# note:  with xls libraries in python - we cannot append to existing files, it will overwrite the existing
# so if we want persistence of data, we may need to add a date/time to the workbook name
# TODO - feature to add data/time to the workbook name.  Have a boolean flag to turn persistence on/off
path_to_runner = "./Debug/SCP.exe"
path_to_runner_x64 = "./x64/Debug/SCP.exe"
path_to_test_files = "./test_data/"
path_to_output = "./output/"
summary_output_file = "summary.xlsx"
path_to_input_files = "./input/"
best_known_input_file = "best-known.txt"
report_average = False

#defines
BEST_COVER = 0
BEST_TIME = 1
AVERAGE_COVER = 2
AVERAGE_TIME = 3



# This class provides the functionality for a switch statement
# found at http://code.activestate.com/recipes/410692/
class switch(object):
    def __init__(self, value):
        self.value = value
        self.fall = False

    def __iter__(self):
        """Return the match method once, then stop"""
        yield self.match
        raise StopIteration

    def match(self, *args):
        """Indicate whether or not to enter a case suite"""
        if self.fall or not args:
            return True
        elif self.value in args: # changed for v1.5, see below
            self.fall = True
            return True
        else:
            return False



def launch_scp_program(path_to_runner, filename, heuristic_code, number_of_runs, coverings_summary):
	print("Launching SCP with {}, code {}, for {} iterations" .format(filename, heuristic_code, number_of_runs))

	# Start the process with the filename and args
	# output will be captured in a list
	test_scp = subprocess.Popen([path_to_runner, path_to_test_files + filename, heuristic_code, number_of_runs, "0"], stdout=subprocess.PIPE)
	output = test_scp.communicate()
	try:
		output_list = output[0].split()
		coverings_summary[BEST_COVER].append(float(output_list[BEST_COVER]))
		coverings_summary[BEST_TIME].append(float(output_list[BEST_TIME]))
		if report_average:
			coverings_summary[AVERAGE_COVER].append(float(output_list[AVERAGE_COVER]))
			coverings_summary[AVERAGE_TIME].append(float(output_list[AVERAGE_TIME]))
	except ValueError:
		print("ValueError:  could not process the test case due to\n\n{}\n" .format(output))
		exit()
	except Exception, e:
		print("OtherError:  could not process the test case due to\n\n{}\n\n" .format(output))
		print(e)
		exit()

def host_path():
	if os.path.exists(path_to_runner):
		return path_to_runner
	elif os.path.exists(path_to_runner_x64):
		return path_to_runner_x64
	else:
		print(
		"path to SCP executable not found\nenvironment settings are not correct\ncontact your support administrator for further details")
		exit()


# will print a single heurtistic's output to a work book
def print_single_heuristic_summary_to_xlsx(worksheet_name, output_summary):
	local_time = time.localtime(time.time())
	time_string = "{}{}{}_{}{}_" .format(local_time[0],local_time[1],local_time[2],local_time[3],local_time[4])
	input_file = open(path_to_input_files + best_known_input_file, 'r')
	workbook = xlsxwriter.Workbook(path_to_output + time_string + summary_output_file)
	add_worksheet(workbook, worksheet_name, output_summary[BEST_COVER], output_summary[BEST_TIME], output_summary[BEST_COVER], output_summary[BEST_TIME], input_file)
	workbook.close()

# will bring all of the heuristics output to a workbook.  one sheet per heuristic.
def print_ALL_heuritic_summary_to_xlsx(random, greedy, local_1, local_2, single_point, population):
	input_file = open(path_to_input_files + best_known_input_file, 'r')
	workbook = xlsxwriter.Workbook(path_to_output + summary_output_file)
	add_worksheet(workbook, "random", random[BEST_COVER], random[BEST_TIME], random[AVERAGE_TIME], random[AVERAGE_COVER], input_file)
	add_worksheet(workbook, "greedy", greedy[BEST_COVER], greedy[BEST_TIME], greedy[AVERAGE_TIME], greedy[AVERAGE_COVER], input_file)
	add_worksheet(workbook, "local_1", local_1[BEST_COVER], local_1[BEST_TIME], local_1[AVERAGE_TIME], local_1[AVERAGE_COVER], input_file)
	add_worksheet(workbook, "local_2", local_2[BEST_COVER], local_2[BEST_TIME], local_2[AVERAGE_TIME], local_2[AVERAGE_COVER], input_file)
	add_worksheet(workbook, "single_point", single_point[BEST_COVER], single_point[BEST_TIME], single_point[AVERAGE_TIME], single_point[AVERAGE_COVER], input_file)
	add_worksheet(workbook, "population", population[BEST_COVER], population[BEST_TIME], population[AVERAGE_TIME], population[AVERAGE_COVER], input_file)
	workbook.close()



def add_worksheet(workbook, heuristic, best_cover_cost, best_cover_time, average_cover_cost, average_cover_time, input_file):
	worksheet = workbook.add_worksheet(heuristic)
	emphasis_formatting = workbook.add_format({'bold': True, 'bg_color': '#C0C0C0', 'border': True})
	add_headings(worksheet, emphasis_formatting)
	fill_sheet(worksheet, emphasis_formatting, best_cover_cost, best_cover_time, average_cover_cost, average_cover_time, input_file)


def add_headings(worksheet, emphasis_formatting):
	worksheet.write('A1', "test id", emphasis_formatting)
	worksheet.write('A2', "best known", emphasis_formatting)
	worksheet.write('A3', "best cost", emphasis_formatting)
	worksheet.write('A4', "time (seconds)", emphasis_formatting)
	worksheet.write('A5', "performance gains (%)", emphasis_formatting)
	if report_average:
		worksheet.write('A6', "average cost", emphasis_formatting)
		worksheet.write('A7', "time (seconds)", emphasis_formatting)
		worksheet.write('A8', "performance gains (%)", emphasis_formatting)
	worksheet.set_column(0, 0, 25)


def fill_sheet(worksheet, emphasis_formatting, best_cover_cost, best_cover_time, average_cover_cost, average_cover_time, input_file):
	row = 0
	i = 1  # i matches column reference workbook.  workbook is row/col indexed at 0, but we have headers in col 0
		   # use i-1 for accessing data in lists

	for line in input_file:
		input = line.split()

		# add test case and best known
		worksheet.write(row, i, input[0], emphasis_formatting)
		worksheet.write_number(row + 1, i, float(input[1]), emphasis_formatting)

		# add best cover cost from program and associated time
		worksheet.write_number(row + 2, i, best_cover_cost[i - 1])
		worksheet.write_number(row + 3, i, best_cover_time[i - 1])

		# generate formula to show the performance gains as a percentage
		best_known_cell = xl_rowcol_to_cell(row + 1, i)
		best_cover_cell = xl_rowcol_to_cell(row + 2, i)
		performance_gains_formula = "=100-((" + best_cover_cell + "/" + best_known_cell + ")*100)"
		worksheet.write_formula(row + 4, i, performance_gains_formula, emphasis_formatting)

		# include average details if necessary
		if report_average:
			# data from runtime
			worksheet.write_number(row + 5, i, average_cover_cost[i - 1])
			worksheet.write_number(row + 6, i, average_cover_time[i - 1])

			# work out performance gains as percentage
			average_cover_cell = xl_rowcol_to_cell(row + 5, i)
			performance_gains_formula = "=100-((" + average_cover_cell + "/" + best_known_cell + ")*100)"
			worksheet.write_formula(row + 7, i, performance_gains_formula, emphasis_formatting)

		i += 1

	# add average performance gains
	worksheet.write(row, i, "average performance gains (%)", emphasis_formatting)
	best_performance_percentage_final_cell = xl_rowcol_to_cell(row + 4, i - 1)
	cell_range = 'A5:' + best_performance_percentage_final_cell
	average_performance_gains_formula = '=SUM(' + cell_range + ')/COUNT(' + cell_range + ')'
	worksheet.write_formula(row + 4, i, average_performance_gains_formula, emphasis_formatting)
	if report_average:
		average_performance_percentage_final_cell = xl_rowcol_to_cell(row + 7, i - 1)
		cell_range = 'A8:' + average_performance_percentage_final_cell
		average_performance_gains_formula = '=SUM(' + cell_range + ')/COUNT(' + cell_range + ')'
		worksheet.write_formula(row + 7, i, average_performance_gains_formula, emphasis_formatting)
	worksheet.set_column(0, i, 25)


# will generate lists to hold the data when running
# after a few goes at writing this, its actually just more maintaibable to set-up all the blank lists, then use
# only what is required.
def generate_data_structures():
	random_heuristic = []
	greedy_heuristic = []
	local_search_tba = []
	local_search__tba = []
	single_point_meta = []
	population_based_meta = []

	best_cover_list = []
	best_time_list = []

	random_heuristic.append(list(best_cover_list))
	random_heuristic.append(list(best_time_list))
	greedy_heuristic.append(list(best_cover_list))
	greedy_heuristic.append(list(best_time_list))
	local_search_tba.append(list(best_cover_list))
	local_search_tba.append(list(best_time_list))
	local_search__tba.append(list(best_cover_list))
	local_search__tba.append(list(best_time_list))
	single_point_meta.append(list(best_cover_list))
	single_point_meta.append(list(best_time_list))
	population_based_meta.append(list(best_cover_list))
	population_based_meta.append(list(best_time_list))

	if report_average:
		average_cover_list = []
		average_time_list = []
		random_heuristic.append(list(average_cover_list))
		random_heuristic.append(list(average_time_list))
		greedy_heuristic.append(list(average_cover_list))
		greedy_heuristic.append(list(average_time_list))
		local_search_tba.append(list(average_cover_list))
		local_search_tba.append(list(average_time_list))
		local_search__tba.append(list(average_cover_list))
		local_search__tba.append(list(average_time_list))
		single_point_meta.append(list(average_cover_list))
		single_point_meta.append(list(average_time_list))
		population_based_meta.append(list(average_cover_list))
		population_based_meta.append(list(average_time_list))

	return random_heuristic, greedy_heuristic, local_search_tba, local_search__tba, single_point_meta, population_based_meta

# Program will run all of the covering problems listed in the test_data folder
# Arguments:
#		1. int:	Represents the operation (section of part 2 of the assignment) to perform
#				1.  Random Constructive
#				2.  Greedy Constructive
#				3.  Local search 1 (tba)
#				4.  Local search 2 (tba)
#				5.  Single Point meta-heuristic
#				6.  Population based meta-heuristic
#				7.  Run ALL currently available heuristics
#		2. int	Number of runs
if __name__ == "__main__":

	if len(sys.argv) < 2:
		print "invalid arguments"

	heuristic_code = sys.argv[1]

	no_runs = sys.argv[2]
	if (int(no_runs) > 2):
		report_average = True
	
	#will hold the details to be written to each sheet of the xlsx
	random_heuristic, greedy_heuristic, local_search_tba, local_search__tba, single_point_meta, population_based_meta = generate_data_structures()

	print("Launching tests\n")
	path_to_runner = host_path()
	for filename in os.listdir(path_to_test_files):
		for case in switch(int(heuristic_code)):
			if case(1):
				launch_scp_program(path_to_runner, filename, heuristic_code, no_runs, random_heuristic)
				break
			if case(2):
				launch_scp_program(path_to_runner, filename, heuristic_code, no_runs, greedy_heuristic)
				break
			if case(3):
				launch_scp_program(path_to_runner, filename, heuristic_code, no_runs, local_search_tba)
				break
			if case(4):
				launch_scp_program(path_to_runner, filename, heuristic_code, no_runs, local_search__tba)
				break
			if case(5):
				launch_scp_program(path_to_runner, filename, heuristic_code, no_runs, single_point_meta)
				break
			if case(6):
				launch_scp_program(path_to_runner, filename, heuristic_code, no_runs, population_based_meta)
				break
			if case(7):
				launch_scp_program(path_to_runner, filename, 1, no_runs, random_heuristic)
				launch_scp_program(path_to_runner, filename, 2, no_runs, greedy_heuristic)
				#still in devel
				#launch_scp_program(path_to_runner, filename, 3, no_runs, local_search_tba)
				#launch_scp_program(path_to_runner, filename, 4, no_runs, local_search__tba)
				#launch_scp_program(path_to_runner, filename, 5, no_runs, single_point_meta)
				#launch_scp_program(path_to_runner, filename, 6, no_runs, population_based_meta)
				break
			if case(): # default
				print "error with input"
				exit()

	print("end tests")
	print("printing summary")
	for case in switch(int(heuristic_code)):
		if case(1):
			print_single_heuristic_summary_to_xlsx("random", random_heuristic)
			break
		if case(2):
			print_single_heuristic_summary_to_xlsx("greedy", greedy_heuristic)
			break
		if case(3):
			print_single_heuristic_summary_to_xlsx("local_1", local_search_tba)
			break
		if case(4):
			print_single_heuristic_summary_to_xlsx("local_2", local_search__tba)
			break
		if case(5):
			print_single_heuristic_summary_to_xlsx("single_point_meta", single_point_meta)
			break
		if case(6):
			print_single_heuristic_summary_to_xlsx("populartion_based", population_based_meta)
			break
		if case(7):
			print_ALL_heuritic_summary_to_xlsx(random_heuristic, greedy_heuristic, local_search_tba, local_search__tba, single_point_meta, population_based_meta)
		if case(): # default
			print "error with input"
			exit()
	print("summary printed - details can be found in %s" % path_to_output + 'summary.xlsx')
