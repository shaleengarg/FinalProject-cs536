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


FILE *fopen(const char *filename, const char *mode){

    FILE *ret;
    ret = real_fopen(filename, mode);
    if(!ret)
        return ret;

    int fd = fileno(ret);


    printf("%s: PID:%d - %s -> %d\n", __func__, getpid(), filename, fd);

    if(reg_file(ret)){
        handle_open(fd, filename);
    }

    return ret;
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){

    //printf("hello_fread, fd:%d\n", fileno(stream));

    // Perform the actual system call
    size_t amount_read = real_fread(ptr, size, nmemb, stream);

#ifdef PREDICTOR
    int fd = fileno(stream); 

    if(reg_file(stream)){ //this is a regular file
        ////lseek doesnt work with f* commands
        
        handle_read(fd, ftell(stream), size*nmemb);
    }
#endif

    return amount_read;
}


ssize_t read(int fd, void *data, size_t size){

    ssize_t amount_read = real_read(fd, data, size);

#ifdef PREDICTOR
    if(reg_fd(fd)){
        //printf("fd: %d lseek: %ld bytes: %lu\n", fd, lseek(fd, 0, SEEK_CUR), size );
        handle_read(fd, lseek(fd, 0, SEEK_CUR), size);
    }
#endif

    return amount_read;
}


size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
    // Perform the actual system call
    size_t amount_written = real_fwrite(ptr, size, nmemb, stream);

    return amount_written;
}


ssize_t write(int fd, const void *data, size_t size){
    ssize_t amount_written = real_write(fd, data, size);
    return amount_written;
}


int fclose(FILE *stream){
    int fd = fileno(stream);
    printf("%s PID:%d fd:%d\n", __func__, getpid(), fd);
#ifdef PREDICTOR
    if(reg_file(stream)){
        handle_close(fd);
    }
#endif
    return real_fclose(stream);
}


int close(int fd){
    printf("File close detected\n");

#ifdef PREDICTOR
    if(reg_fd(fd)){
        //remove from the predictor data
        handle_close(fd);
    }
#endif

    return real_close(fd);
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
               printf("fd:%d block device\n", fd);
               break;
           case S_IFCHR:
               printf("fd:%d character device\n", fd);
               break;
           case S_IFDIR:
               printf("fd:%d directory\n", fd);
               break;
           case S_IFIFO:
               printf("fd:%d FIFO/pipe\n", fd);
               break;
           case S_IFLNK:
               printf("fd:%d symlink\n", fd);
               break;
           case S_IFREG:
               printf("fd:%d regular file\n", fd); 
               return true;            
               break;
           case S_IFSOCK:
               printf("fd:%d socket\n", fd);
               break;
           default:
               printf("fd:%d unknown?\n", fd);
           }
        /*
        if(S_ISREG(st.st_mode)){
            return true;
        }
        */
    }
    //return true;
    return false;
}
