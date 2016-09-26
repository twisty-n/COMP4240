import subprocess
import threading
import os
import csv
import sys
import numpy
import xlsxwriter	# will be highlighted in red because pycharm cannot find library - but is installed, you can see listed using 'pip list', and works fine
					# note:  with xls libraries in python - we cannot append, we have to write a new file each time. Taken into consideration with design



path_to_runner = "./Debug/SCP.exe"
path_to_runner_x64 = "./x64/Debug/SCP.exe"
path_to_test_files = "./test_data/"
path_to_output = "./output/"
summary_output_file = "summary.xlsx"
path_to_input_files = "./input/"
best_known_input_file = "best-known.txt"
no_test_cases = 66


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
	best_known_solution.append(float(output_list[0]))
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


def print_summary():
	workbook = xlsxwriter.Workbook(path_to_output+summary_output_file)
	worksheet = workbook.add_worksheet('testing')

	file = open(path_to_input_files+best_known_input_file, 'r')

	row = 0
	i = 0
	for line in file:
		input = line.split()
		worksheet.write_string(row, i, input[0])
		worksheet.write(row+1, i, input[1])
		#worksheet.write(row+2, i, best_cover_random_cost[i])
		#worksheet.write(row+3, i, best_cover_random_time[i])
		#if report_average:
		#	worksheet.write(row+4, i, average_cover_random_cost[i])
		#	worksheet.write(row+5, i, average_cover_random_time[i])
		i += 1

	workbook.close()

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
	print_summary()
	print("summary printed - details can be found in %s" % path_to_output+'summary.xlsx')
