/*
 * This file takes the decisions on 
 * Prefetching and Demotion/relinquishing of memory
 `*/
#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include "ngram.hpp"
#include "util.hpp"
#include "predictor.hpp"
#include "sequential.hpp"

/* Keeps track of all filenames wrt its corresponding fd*/
std::unordered_map<int, std::string> fd_to_filename;

bool handle_open(int fd, const char *filename){

    if(fd<=2 || filename == NULL)
        return false;

    debug_print("handle_open: fd:%d %s\n", fd, filename);

    return true;
}


int handle_read(int fd, off_t pos, size_t bytes) {
    static struct pos_bytes a;

    if(pos <0 || bytes <=0 || fd <=2) //Santization check
        return false;

    a.fd = fd;
    a.pos = pos;
    a.bytes = bytes;

    debug_print("handle_read: fd:%d, pos:%lu, bytes:%zu\n", 
            fd, pos, bytes);

#ifdef SEQUENTIAL
    seq_readobj.insert(a);
#endif

    return true;
}


int handle_write(int fd, off_t pos, size_t bytes){

    return true;
}


int handle_close(int fd){
    return true;
}
