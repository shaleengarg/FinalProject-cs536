#!/usr/bin/env python3

"""
Window is the number of read instructions that would be considered

We would like the ML model to be able to give a cleanup decision after every WINDOW which is good for 
WINDOW*LOOKAHEAD instructions

meaning if window = 5 and lookahead = 2, then, if the ML model wants to remove this data point, then
ideally it must not be accessed again (by any process) for 10 accesses done by the application
"""

WINDOW = 100
LOOKAHEAD = 2
filename='log-madbench_reformat_042821.csv'

fd = open(filename, 'r')
filecontent = fd.readlines()
nr_lines = len(filecontent)
nr_windows= (nr_lines-1)/100


def main():
    for i in range(1, nr_windows):
        for j in range(i*WINDOW, i)
        
    print(filecontent[i])



if __name__ == "__main__":
    main()
