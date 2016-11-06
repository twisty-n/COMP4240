import subprocess
import threading
import os
import csv
import sys
import numpy
import xlsxwriter
from xlsxwriter.utility import xl_rowcol_to_cell  #used to generate A1 notations of cells

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


# lists are required to store data - as this works best with the
# csv module - however, I'm not sure if this is going to be the best
# way to track the data.  We would need one list per row.
# I will look into iterable data structures for storing and writing to csv
# but at the moment, this is a 'working' csv write solution

test_id = []
# rows in data:
best_known_solution = []
best_cover_random_cost = []
best_cover_random_time = []
average_cover_random_cost = []
average_cover_random_time = []

report_average = False


def launch_scp_program(path_to_runner,filename):
	print("Launching SCP with %s" % filename)
	# Start the process with the filename and args
	# output will be captured in a list
	test_scp = subprocess.Popen([path_to_runner, path_to_test_files+filename, "test"],  stdout=subprocess.PIPE)
	output = test_scp.communicate()
	output_list = output[0].split()
	# then added to to the summary lists
	best_cover_random_cost.append(float(output_list[0]))
	best_cover_random_time.append(float(output_list[1]))
	if report_average:
		average_cover_random_cost.append(float(output_list[2]))
		average_cover_random_time.append(float(output_list[3]))


def host_path():
	if os.path.exists(path_to_runner):
		return path_to_runner
	elif os.path.exists(path_to_runner_x64):
		return path_to_runner_x64
	else:
		print("path to SCP executable not found\nenvironment settings are not correct\ncontact your support administrator for further details")
		exit()

#for debug
def print_lists_to_console():
	print "best_cover_list: ", best_cover_random_cost
	print "best_cover_time_list: ", best_cover_random_time
	if report_average:
		print "average_cover_list: ", average_cover_random_cost
		print "average_cover_time: ", average_cover_random_time


def append_to_csv():
	with open(path_to_output+summary_output_file, 'a') as csvfile:
		wr = csv.writer(csvfile, dialect='excel', delimiter=',', quoting=csv.QUOTE_NONE, quotechar='', escapechar='')
		wr.writerow(best_cover_random_cost)
		wr.writerow(best_cover_random_time)
		if report_average:
			wr.writerow(average_cover_random_cost)
			wr.writerow(average_cover_random_time)
	csvfile.close()


def print_summary_to_xlsx():

	workbook = xlsxwriter.Workbook(path_to_output+summary_output_file)
	add_worksheet(workbook, "random constructive heuristic", best_cover_random_cost, best_cover_random_time, average_cover_random_cost, average_cover_random_time)
	#will be able to add more sheets based on the other heuristics from here
	workbook.close()

def add_worksheet(workbook, heuristic, best_cover_cost, best_cover_time, average_cover_cost, average_cover_time):
	worksheet = workbook.add_worksheet(heuristic)
	emphasis_formatting = workbook.add_format({'bold': True, 'bg_color': '#C0C0C0', 'border': True})
	add_headings(worksheet, emphasis_formatting)
	fill_sheet(worksheet, emphasis_formatting, best_cover_cost, best_cover_time, average_cover_cost, average_cover_time) #think this is pass by reference, so think it might be okay


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

def fill_sheet(worksheet, emphasis_formatting, best_cover_cost, best_cover_time, average_cover_cost, average_cover_time):

	# TODO:  Lauren - fix this so that you are not loading and reading from file each time - here now only because it 'works'
	input_file = open(path_to_input_files+best_known_input_file, 'r')

	row = 0
	i = 1			# i matches column reference workbook.  workbook is row/col indexed at 0, but we have headers in col 0
					# use i-1 for accessing data in lists

	for line in input_file:
		input = line.split()

		#add test case and best known
		worksheet.write(row, i, input[0], emphasis_formatting)
		worksheet.write_number(row+1, i, float(input[1]), emphasis_formatting)

		#add best cover cost from program and associated time
		worksheet.write_number(row+2, i, best_cover_cost[i-1])
		worksheet.write_number(row+3, i, best_cover_time[i-1])

		#generate formula to show the performance gains as a percentage
		best_known_cell = xl_rowcol_to_cell(row+1,i)
		best_cover_cell = xl_rowcol_to_cell(row+2,i)
		performance_gains_formula = "=100-(("+best_cover_cell+"/"+best_known_cell+")*100)"
		worksheet.write_formula(row+4, i, performance_gains_formula, emphasis_formatting)

		#include average details if necessary
		if report_average:

			#data from runtime
			worksheet.write_number(row+5, i, average_cover_cost[i-1])
			worksheet.write_number(row+6, i, average_cover_time[i-1])

			#work out performance gains and as percentage
			average_cover_cell = xl_rowcol_to_cell(row+5,i)
			performance_gains_formula = "=100-(("+average_cover_cell+"/"+best_known_cell+")*100)"
			worksheet.write_formula(row+7, i, performance_gains_formula, emphasis_formatting)

		i += 1

	#add average performance gains
	worksheet.write(row, i, "average performance gains (%)", emphasis_formatting)
	best_performance_percentage_final_cell = xl_rowcol_to_cell(row+4,i-1)
	cell_range = 'A5:'+best_performance_percentage_final_cell
	average_performance_gains_formula = '=SUM('+cell_range+')/COUNT('+cell_range+')'
	worksheet.write_formula(row+4, i, average_performance_gains_formula, emphasis_formatting)
	if report_average:
		average_performance_percentage_final_cell = xl_rowcol_to_cell(row+7,i-1)
		cell_range = 'A8:'+average_performance_percentage_final_cell
		average_performance_gains_formula = '=SUM('+cell_range+')/COUNT('+cell_range+')'
		worksheet.write_formula(row+7, i, average_performance_gains_formula, emphasis_formatting)
	worksheet.set_column(0, i, 25)


if __name__ == "__main__":

	if len(sys.argv) > 1:
		no_runs = sys.argv[1]
		report_average = True

	print("Launching tests\n")
	path_to_runner = host_path()
	for filename in os.listdir(path_to_test_files):
		launch_scp_program(path_to_runner, filename)
		# thread = threading.Thread(target=launch_scp_program, args=(filename,))
		# thread.daemon = True
		# thread.start()

	#  print_lists_to_console()
	#  append_to_csv()
	print("end tests")

	print("printing summary")
	print_summary_to_xlsx()
	print("summary printed - details can be found in %s" % path_to_output+'summary.xlsx')
