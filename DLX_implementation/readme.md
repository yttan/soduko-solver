### Python implementation of Algorithm X

This code requires a machine with Anaconda Python 3.6 installed

#### instructions

1. Change working directory to the location of these files

2. Run in a terminal

>> ipython

3. Type in ipython:

>> %run Xsolver.py

4. Select sudoku file from the benchmark/Sudoku folder and run the Xsolver using.
   For example:

>> cd benchmarks/Sudoku/
>> file = "hexa-101.txt"
>> Xsolver(file,size=16,print_sol=True).load_sudoku_and_run()

5. For printing all the sudoku solutions load all the sudoku files into a list

>> from os import listdir
>> benchfiles = listdir()
>> benchfiles

>> for file in benchfiles:
>>    print(file)
>>    Xsolver(file,size=16,print_sol=True).load_sudoku_and_run()

5. For exporting the sudoku solution to a txt file individually

>> for file in benchfiles:
>>    print(file)
>>    Xsolver(file,size=16,print_sol=True, write_solution_file=True).load_sudoku_and_run()

6. Finally, for timing the code we suggest using the %timeit ipython command
   that tests lines of code. This should a ~2 minutes to run. Each Sudoku
   is solved 7x100 times and the best runs are averaged out. There will be an
   estimated mean and std for the time required to solve each Sudoku
   individually. The parameter -n can be increased to 100 for a better 
   estimation of the timing.

>> for file in benchfiles:
>>     print(file)
>>     %timeit -n 10 Xsolver(file,size=16,print_sol=False).load_sudoku_and_run()
