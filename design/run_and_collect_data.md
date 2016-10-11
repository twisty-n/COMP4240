#run.py proposal

##objectives

Python will be used to run all instances of scp residing in /test, then create an executive summary in csv which can be used to quickly create charts for the report 

##plan

	1.	run.py will will load each instance of scp provided - residing in /test_data

	2.	for each instance all heuristics will be run (assuming this will be 6 - but will begin with just the one for now)

		2a.	python will store best found covering, and time to cover test case for each heuristc

	3. one finished (for all scp instaces in /test_data) python will then append summary of details to the .csv
			
			3a. in /output will be a best_known_master.csv - which holds the best now for each of the data files - python will take a copy of this, and create the lastest run .csv.   12 columns will be appended,  best solution and time for each heuristic.  

	4. at this point, python may then run some manipulations on the csv to help identify average run times and best runs etc 
	
		4a.  this may be expanded, to then identify the best performer for each heuristic and then generate a solution output file for that particular instance
	
##notes

This .csv if for storing executive summary data that can be used in the reports.  for the first round of the development, it is simple (i.e; will address points 1, 2 and 3), but if it needs to be expanded, it should be easy enough to do so.

Log files with more data for each heuristic can (and probably should) still exist, but these can be built in c and turned on/off during devleopment as needed.