#!/usr/bin/python3

import sys

if len(sys.argv) != 2:
    print("Enter a output file name")
    exit(1)

infile = open(sys.argv[1], "r")

lines = infile.readlines()

unique_files_set = set()
access_count = {}

for line in lines:
    filename = line[line.index("/"):].strip()
    #print(filename)
    
    if filename not in access_count:
        unique_files_set.add(filename)
        access_count[filename] = 1
    else:
        if filename not in access_count:
            print(filename + " not present????")
            print(access_count)
        access_count[filename] += 1

unique_files_list = list(unique_files_set)
unique_files_list.sort()

for file in unique_files_list:
    print(file + " touched " + str(access_count[file]) + " times.")

infile.close()
