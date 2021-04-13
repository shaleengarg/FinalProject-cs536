#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <errno.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <map>
#include <deque>
#include <unordered_map>
#include <string>
#include <iterator>

#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "frontend.hpp"
#include "handler.hpp"

static void con() __attribute__((constructor));
static void dest() __attribute__((destructor));


void con(){
    printf("init tracing...\n");

    open_sem();
}


void dest(){
    printf("application termination...\n");
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){

    // Perform the actual system call
    size_t amount_read = real_fread(ptr, size, nmemb, stream);

    int fd = fileno(stream); 

    if(reg_file(stream)){ //this is a regular file
        ////lseek doesnt work with f* commands
        
        handle_read(fd, ftell(stream), size*nmemb);
    }

    return amount_read;
}


ssize_t read(int fd, void *data, size_t size){

    ssize_t amount_read = real_read(fd, data, size);

    if(reg_fd(fd)){
        handle_read(fd, lseek(fd, 0, SEEK_CUR), size);
    }

    return amount_read;
}


size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
    // Perform the actual system call
    size_t amount_written = real_fwrite(ptr, size, nmemb, stream);

    int fd = fileno(stream); 
    if(reg_file(stream)){ //this is a regular file
        handle_write(fd, ftell(stream), size*nmemb);
    }

    return amount_written;
}


ssize_t write(int fd, const void *data, size_t size){
    ssize_t amount_written = real_write(fd, data, size);

    if(reg_fd(fd)){
        handle_write(fd, lseek(fd, 0, SEEK_CUR), size);
    }
    return amount_written;
}


//returns fd if  FILE is a regular file
int reg_file(FILE *stream){
    return reg_fd(fileno(stream));
}

//returns true if fd is regular file
bool reg_fd(int fd)
{
    if(fd<=2)
        return false;

    struct stat st;

    if(fstat(fd, &st) == 0){
        switch (st.st_mode & S_IFMT) {
           case S_IFBLK:
               break;
           case S_IFCHR:
               break;
           case S_IFDIR:
               break;
           case S_IFIFO:
               break;
           case S_IFLNK:
               break;
           case S_IFREG:
               return true;            
               break;
           case S_IFSOCK:
               break;
           default:
               printf("fd:%d unknown?\n", fd);
           }
    }
    return false;
}
