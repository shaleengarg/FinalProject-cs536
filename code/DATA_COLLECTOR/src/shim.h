#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/uio.h>
#include <sys/mman.h>

#include <libexplain/fopen.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <list>
#include <set>
using namespace std;


// seperate into facilities:
// - setup (setup.c) - setups up and breaksdown shim
// - impl (impl.c) - implementation of shim functions
// - debug (debug.c) - handles debugging
// - environment (env.c) - helper functions for shim environment
// - info (info.c) - utility of various helper fuctions 
// - ignore (ingore.c) - handles ignoring of files
// - link (link.c) - links real functions

// Link functions  (link.c)
extern void link_real_functions(void);

// Debug facility functions (debug.c)
extern void debug(const char *format, ...);
extern void debug_setup(void);
extern void debug_breakdown(void);

// Env factility function (env.c)
extern string get_xml_export_path(void);
extern string get_xml_export_directory(void);
extern string get_xml_export_prefix(void);
extern string get_xml_export_name(void);
extern string get_run_name(void);
extern string get_run_description(void);
extern list<string> get_intermediate_file_paths(void);
extern string get_target_gppid();
extern bool is_root_process(void);
extern string get_debug_output_path(void);

// Info facility functions (info.c)
extern void print_filename_fd(int fd);
extern void print_filename_file(FILE *stream);
extern void print_info_fd(int fd);
extern void print_info_file(FILE *stream);
extern string filename_via_fd(int fd);
extern string filename_via_file(FILE *stream);
extern string get_process_name(const pid_t pid);
extern string  get_process_command(const pid_t pid);
extern pid_t getgppid(void);

// Ignore facility functions (ignore.c)
extern void ignore_path(string path);
extern int should_we_ignore(const char *path);
extern int should_we_ignore(string path);
extern int ignored_fd(int fd);
extern int ignored_file(FILE *stream);
extern int ignored_filename(const char *path);
extern void ignore_fd(int fd);
extern void ignore_file(FILE *stream);
extern void unignore_fd(int fd);
extern void unignore_file(FILE *stream);
extern bool file_exists(const std::string& name);

// Files facility (files.c)
extern void start_tracing(void);
extern void stop_tracing(void);
extern void inc_file_op_filename(string filname, string op, string attr, int value);
extern void inc_file_op_fd(int fd, string op, string attr, int value);
extern void inc_file_op_file(FILE *stream, string op, string attr, int value);
extern void print_files(void);
extern void export_process_stats(void);
extern void aggregate_stats(void);
extern void print_ignored_files(void);
extern void delete_ignored_file_lists(void);
extern set<string> get_ignored_files(void);

// extern pointers to real functions
extern int (*real_open)(const char *pathname, int flags, ...);
extern int (*real_open64)(const char *pathname, int flags, ...);
extern int (*real_openat)(int dirfd, const char *pathname, int flags, ...);
extern int (*real_close)(int fd);
extern int (*real_truncate)(const char *path, off_t length);
extern int (*real_ftruncate)(int fd, off_t length);
extern int (*real_truncate64)(const char *path, off_t length);
extern int (*real_ftruncate64)(int fd, off_t length);
extern int (*real_fallocate)(int fd, int mode, off_t offset, off_t len);
extern int (*real_creat)(const char *pathname, mode_t mode);
extern int (*real_posix_fallocate)(int fd, off_t offset, off_t len);
extern int (*real_sync_file_range)(int fd, off64_t offset, off64_t nbytes, unsigned int flags);
extern ssize_t (*real_readahead)(int fd, off64_t offset, size_t count);
extern FILE* (*real_fopen)(const char *path, const char *mode);
extern FILE* (*real_fdopen)(int fd, const char *mode);
extern FILE* (*real_freopen)(const char *path, const char *mode, FILE *stream);
extern int (*real_fclose)(FILE *fp);
extern ssize_t (*real_read)(int fd, void *buf, size_t count);
extern ssize_t (*real_write)(int fd, const void *buf, size_t count);
extern ssize_t (*real_pread)(int fd, void *buf, size_t count, off_t offset);
extern ssize_t (*real_pwrite)(int fd, const void *buf, size_t count, off_t offset);
extern ssize_t (*real_pread64)(int fd, void *buf, size_t count, off_t offset);
extern ssize_t (*real_pwrite64)(int fd, const void *buf, size_t count, off_t offset);
extern size_t (*real_fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);
extern size_t (*real_fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream);
extern ssize_t (*real_readv)(int fd, const struct iovec *iov, int iovcnt);
extern ssize_t (*real_writev)(int fd, const struct iovec *iov, int iovcnt);
extern ssize_t (*real_preadv)(int fd, const struct iovec *iov, int iovcnt, off_t offset);
extern ssize_t (*real_pwritev)(int fd, const struct iovec *iov, int iovcnt, off_t offset);
extern off_t (*real_lseek)(int fd, off_t offset, int whence);
extern int (*real_fseek)(FILE *stream, long offset, int whence);
extern long (*real_ftell)(FILE *stream);
extern void (*real_rewind)(FILE *stream);
extern int (*real_fgetpos)(FILE *stream, fpos_t *pos);
extern int (*real_fsetpos)(FILE *stream, const fpos_t *pos);
extern int (*real_fgetc)(FILE *stream);
extern char* (*real_fgets)(char *s, int size, FILE *stream);
extern int (*real_getc)(FILE *stream);
extern int (*real_ungetc)(int c, FILE *stream);
extern int (*real_fputc)(int c, FILE *stream);
extern int (*real_fputs)(const char *s, FILE *stream);
extern int (*real_putc)(int c, FILE *stream);
extern int (*real_fsync)(int fd);
extern int (*real_fdatasync)(int fd);
extern void (*real_sync)(void);
extern int (*real_syncfs)(int fd);
extern int (*real_fflush)(FILE *stream);
extern int (*real_msync)(void *addr, size_t length, int flags);
extern void* (*real_mmap)(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
extern void* (*real_mmap2)(void *addr, size_t length, int prot, int flags, int fd, off_t pgoffset);
extern int (*real_munmap)(void *addr, size_t length);
extern char* (*real_mmap_private)(const char* filename,size_t* filesize);
extern char* (*real_mmap_read)(const char* filename,size_t* filesize);
extern char* (*real_mmap_shared)(const char* filename,size_t* filesize);
