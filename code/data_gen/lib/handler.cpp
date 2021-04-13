/*
 * This file takes the decisions on 
 * Prefetching and Demotion/relinquishing of memory
 */
#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include <semaphore.h>

#include "handler.hpp"

sem_t   *mysemp;
const char semname[] = "mysem";
const char logname[] = "log-madbench";


int handle_read(int fd, off_t pos, size_t bytes) {
    if(pos <0 || bytes <=0 || fd <=2) //Santization check
        return false;

    get_sem();
    write_log(0, getpid(), fd, pos, bytes);
    post_sem();
    return true;
}


int handle_write(int fd, off_t pos, size_t bytes){
    if(pos <0 || bytes <=0 || fd <=2) //Santization check
        return false;

    get_sem();
    write_log(1, getpid(), fd, pos, bytes);
    post_sem();

    return true;
}


bool open_sem(){
    mysemp = sem_open(semname, O_CREAT, 0666, 1);
    if (mysemp == SEM_FAILED) {
        printf("sem_open() failed %s\n", strerror(errno));
        return false;
    }
    return true;
}

bool get_sem(){
    int sts = sem_wait(mysemp);
    if(sts == 0)
        return true;
    return false;
}

bool post_sem(){
    int sts = sem_post(mysemp);
    if(sts == 0)
        return true;
}

bool write_log(int type, pid_t pid, int fd, off_t offset, size_t bytes){
    FILE * fp;
    fp = fopen (logname, "a");

    if(fp == NULL)
    {
        printf("Unable to open file\n");
        return -1;
    }

    fprintf(fp, "%d, PID:%d, FD:%d, OFFSET:%lu, SIZE:%zu\n",
            type, pid, fd, offset, bytes);

    fclose(fp);
    return(0);
}
