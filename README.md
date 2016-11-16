# COMP4240 - c3068332 (Lauren Carter) && c


##Python Script

###About
	The python script run.py will run SCP.exe for all instances of the test cases found in ./test_data
	The SCP.exe program provides executive level data which python will pipe to a .xlsx report

###Version
	Developed using:
	Python 2.7.12 :: Anaconda 4.2.0 (64-bit)
	Available at:	https://www.continuum.io/downloads
	
	
###Install
	
####Extra python libraries required for run.py
	xlsxwriter
	You can check if it is already in your environment using command:
		pip list
	if not already installed - it can be installed with command:
		
	Note:  if you review the file using pycharm, it is possible any xlsxwriter functions are highlighted in red, however the program should actually work fine ((from console))
	
####Command console
	when using windows, the best console to use with this program is: cmder
	Available at: http://cmder.net/
	note: the program will run fine in CMD, but you'll miss out on the prettiness and any text output from python is padded with formatting codes

###Running
	
####Input params
	1. int:	Represents the operation or heuristic to perform
		1.  Random Constructive
		2.  Greedy Constructive
		3.  Local search best fit
		4.  Local search first fit
		5.  Simulated Annealing
		6.  Jumping Particle Swarm
		7.  Run ALL currently available heuristics (will take some time)
	2. int	Number of runs
	
####Run command
	python run.py <int_1> <int_2>
	
####Unavailable Features
  Printing the raw output of a solution to file does not run when using the python script.
  The python script can only produce an executive level report.
  If granular raw output of the solutions is desired, this feature is available when runing SCP.exe from the visual studio environment.
  To turn this feature on, please see notes below in the section SCP.exe 
 
 
  




