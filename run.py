import subprocess
import threading
import os
import csv
import sys


path_to_runner = "./Debug/SCP.exe"
path_to_runner_x64 = "./x64/Debug/SCP.exe"
path_to_test_files = "./test_data/"
path_to_output = "./output/"
summary_output_file = "summary.csv"


# lists are required to store data - as this works best with the
# csv module - however, I'm not sure if this is going to be the best
# way to track the data.  We would need one list per row.
# I will look into iterable data structures for storing and writing to csv
# but at the moment, this is a 'working' csv write solution
best_cover_random_cost = ['random - best cover']
best_cover_random_time = ['random - best_time']
average_cover_random_cost = ['random - average_cover']
average_cover_random_time = ['random - average_time']
report_average = False



def launch_scp_program(path_to_runner,filename):
    print("Launching SCP with %s" % filename)
    # Start the process with the filename and args

    # output will be captured in a list
    output_list = []
    test_scp = subprocess.call([path_to_runner, path_to_test_files+filename, "test"], stdout=output_list)

    # then added to to the summary lists
    best_cover_random_cost.append(output_list[0])
    best_cover_random_time.append(output_list[1])
    if report_average:
        average_cover_random_cost.append(output_list[2])
        average_cover_random_time.append(output_list[3])


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
    print "average_cover_list: ", average_cover_random_cost
    print "average_cover_time: ", average_cover_random_time


def append_to_csv():
    with open(path_to_output+summary_output_file, 'a') as csvfile:
        wr = csv.writer(csvfile, dialect='excel', delimiter=',', quoting=csv.QUOTE_NONE, quotechar='', escapechar='')
        wr.writerow(best_cover_random_cost)
        wr.writerow(best_cover_random_time)
        wr.writerow(average_cover_random_cost)
        wr.writerow(average_cover_random_time)
    csvfile.close()

if __name__ == "__main__":

    print("Launching tests\n")
    path_to_runner = host_path()
    for filename in os.listdir(path_to_test_files):
       launch_scp_program(path_to_runner,filename)
       #thread = threading.Thread(target=launch_scp_program, args=(filename,))
       #thread.daemon = True
       #thread.start()

    append_to_csv()

    print("end tests")
