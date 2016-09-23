import subprocess
import threading
import os


path_to_runner = "./x64/Debug/SCP.exe"
#path_to_runner = "./Debug/SCP.exe"
path_to_test_files = "./test_data/"
path_to_output = "./output/"

def launch_scp_program(filename):
    print("Launching SCP with %s" % filename)
    # Start the process with the filename and args
    subprocess.call([path_to_runner, path_to_test_files+filename, "test"])
    # Listen to the results


    # Notify with any output


if __name__ == "__main__":
    print("Launching tests")
    for filename in os.listdir(path_to_test_files):
        launch_scp_program(filename)
       #thread = threading.Thread(target=launch_scp_program, args=(filename,))
       #thread.daemon = True
       #thread.start()
