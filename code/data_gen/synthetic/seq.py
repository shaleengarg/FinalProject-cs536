#!/usr/bin/env python3

"""
Generates simple seq and strided access patterns

"""

from random import seed
from random import randint
import random

nr_entries=5000000
stride=1
nr_files=10
seed(1)

READ='0'
DELIM=","
outfilename='str-1_files-10_ent-500k.csv'

dict_fd_offset = {}
dict_fd_size = {}
fd_list = []

def write_outfile(line):
    try:
        f = open(outfilename, "a")
        outline = line + '\n'
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

def init_dicts():
    for _ in range(nr_files):
        fd = randint(1000, 2000)
        fd_list.append(fd)
        dict_fd_offset[fd] = 0

    for key in dict_fd_offset:
        dict_fd_size[key] = randint(4096, 1048576) #4KB to 1MB


def generate_line():
    fd = random.choice(fd_list)
    pid = randint(1000, 5000) #PID value
    ret_line = READ + DELIM 
    ret_line += str(pid) + DELIM
    ret_line += str(fd) + DELIM
    ret_line += str(dict_fd_offset[fd]) + DELIM
    ret_line += str(dict_fd_size[fd])
    dict_fd_offset[fd] += dict_fd_size[fd]*stride #move stride lenght
    return ret_line


def main():
    init_dicts()
    for i in range(nr_entries):
        line = generate_line()
        write_outfile(line);


if __name__ == "__main__":
    main()
