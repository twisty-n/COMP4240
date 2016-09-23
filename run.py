import subprocess
import threading
import os
import csv


path_to_runner = "./Debug/SCP.exe"
path_to_runner_x64 = "./x64/Debug/SCP.exe"
path_to_test_files = "./test_data/"
path_to_output = "./output/"

def launch_scp_program(path_to_runner,filename):
    print("Launching SCP with %s" % filename)
    # Start the process with the filename and args
    subprocess.call([path_to_runner, path_to_test_files+filename, "test"])
    # Listen to the results


    # Notify with any output


def host_path():
    if os.path.exists(path_to_runner):
        return path_to_runner
    elif os.path.exists(path_to_runner_x64):
        return path_to_runner_x64
    else:
        print("path to SCP executable not found\nenvironment settings are not correct\ncontact your support administrator for further details")
        exit()

if __name__ == "__main__":

    print("Launching tests\n")
    path_to_runner = host_path()
    for filename in os.listdir(path_to_test_files):
       launch_scp_program(path_to_runner,filename)
       #thread = threading.Thread(target=launch_scp_program, args=(filename,))
       #thread.daemon = True
       #thread.start()

    print("end tests")
