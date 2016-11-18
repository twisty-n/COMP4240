# COMP4240 - c3068332 (Lauren Carter) && c3163181 (Tristan Newmann)


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
 

##SCP.exe

###About
	This program will load one instance of the set covering problem.
	The instance should be proivded as a text file, match the the format supplied with the assignment breif
	This program can generate solutions using one of 6 heuristics
		Random construction
		Greedy construction
		Local search best accept
		Local search first accept
		Simulated annealing
		Jumping particle Swarm
	More information about these heuristics can be found in the report provided

###Version
	Developed using:
	Visual Studion 2015 - using provided c++ libraries

###Running
	
####Input params
	1. string:		Filename representing the input file to deal with
 	2. int:			Represents the operation (section of part 2 of the assignment) to perform
					1.  Random Constructive
					2.  Greedy Constructive
					3.  Local search 1 (tba)
					4.  Local search 2 (tba)
					5.  Single Point meta-heuristic
					6.  Population based meta-heuristic
	3. int			Number of runs
	4. int			Will be used to indicate debug mode. 1 == TRUE 0 == FALSE
	
####Run command
	python run.py <int_1> <int_2>

####Raw output
	This program can generate some raw output for the best solution it was about to generate.  To turn this feature on.  To generate raw output, simply run the program in debug mode. Raw output of solution details will be found in the folder \output\raw_dump\
