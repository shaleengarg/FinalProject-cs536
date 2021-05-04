#!/usr/bin/env python3

"""
Generates randomly re used access patterns

"""

from random import seed
from random import randint
import random

nr_entries=5000000
window_sz=100
randomness=0.4 ##randomly, some of the data points will be reused
stride=3
nr_files=10
outfilename='rand_reuse_str-3_files-10_ent-500k.csv'

nr_windows = int(nr_entries/window_sz)

seed(1)

READ='0'
DELIM=","

dict_fd_offset = {}
dict_fd_size = {}
fd_list = []
access_list = []

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


def first_time():
    for j in range(window_sz):
        line = generate_line()
        access_list.append(line);
        write_outfile(line);

def random_reuse():
    for j in range(window_sz):
        #flip a biased coin, either generate_line or reuse the same one
        flip = random.randint(1, 100)
        if(flip < 100*randomness): ##Reuse
            line = access_list[j]
        else:
            line = generate_line()
        write_outfile(line)


def main():
    init_dicts()
    for i in range(nr_windows):
        if i == 0:
            first_time()
        else:
            random_reuse()
    print(access_list)


if __name__ == "__main__":
    main()
