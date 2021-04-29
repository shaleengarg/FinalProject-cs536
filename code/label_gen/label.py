#!/usr/bin/env python3

"""
Window is the number of read instructions that would be considered

We would like the ML model to be able to give a cleanup decision 
after every WINDOW which is good for the next WINDOW*LOOKAHEAD instructions

meaning if window = 5 and lookahead = 1, then, if the ML model wants to remove this data point, then
ideally it must not be accessed again (by any process) for next 5  accesses done by the application after the window
"""

WINDOW = 100
LOOKAHEAD = 2
filename='log-madbench_reformat_042821.csv'
outfilename='labeled_log-madbench_reformat.csv'
DELIM=','

fd = open(filename, 'r')
filecontent = fd.readlines()
nr_lines = len(filecontent)
nr_windows= int((nr_lines-1)/100)

def write_outfile(line, label):
    try:
        f = open(outfilename, "a")
        outline = line + DELIM + label + "\n"
        f.write(outline)

    except IOError:
        print("Error: Unable to open/write to file", outfilename)
        return False

    finally:
        f.close()
        


#True == access found in the next accesses
def check_access(check_fd, check_offset, start_line, check_nr_lines):
    end_line = start_line+check_nr_lines
    if end_line > nr_lines:
        end_line = nr_lines
    for i in range(start_line, end_line):
        access = filecontent[i].split(DELIM)
        if(access[2] == check_fd and access[3] == check_offset):
            return True
    return False


def main():
    for i in range(1, nr_windows):
        start = i*WINDOW
        end = (i+1)*WINDOW
        for j in range(start, end):
            line = filecontent[j].split(DELIM)
            if(check_access(line[2], line[3], end, LOOKAHEAD*WINDOW) == True):
                write_outfile(line, "0")
            else:
                write_outfile(line, "1")


if __name__ == "__main__":
    main()
