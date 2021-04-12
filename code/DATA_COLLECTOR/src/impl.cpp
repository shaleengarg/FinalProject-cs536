#include "shim.h"

int open(const char *pathname, int flags, ...){
	int ret;


	debug("[SHIM][open]\t\t\t\t\t\t\t%s\n", pathname);
	//debug("open call not handled\n");

	// If O_CREAT is used to create a file, the file access mode must be given.
	if(flags & O_CREAT){
		va_list valist;
		va_start(valist, flags);
		int mode = va_arg(valist, int);
		va_end(valist);
		ret = real_open(pathname, flags, mode);
	}else{
		ret = real_open(pathname, flags);	
	}

	// check for bad return? do this
	
	// Update statistics
	inc_file_op_filename(pathname, __func__, "calls", 1); // increment by 1

	return ret;
}


int openat(int dirfd, const char *pathname, int flags, ...){
	int ret;

	debug("[SHIM][openat] \t\t\t\t\t\t%s\n", pathname);
	//debug("openat call not handled\n");
	
	if(flags & O_CREAT){
		va_list valist;
		va_start(valist, flags);
		int mode = va_arg(valist, int);
		va_end(valist);
		ret = real_openat(dirfd, pathname, flags, mode);
	}else{
		ret = real_openat(dirfd, pathname, flags);
	}
	
	// Update statistics
	inc_file_op_filename(pathname, __func__, "calls", 1); // increment by 1

	return ret;
}


int close(int fd){

	debug("[SHIM][close]\t\t");
	print_info_fd(fd);
	//debug("close call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 


	return real_close(fd);
}


int truncate(const char *path, off_t length){

	debug("[SHIM][truncate] %s\n", path);
	//debug("truncate call not handled\n");
	
	// Update statistics
	inc_file_op_filename(path, __func__, "calls", 1);
	inc_file_op_filename(path, __func__, "length", length);
	
	return real_truncate(path, length);
}


int ftruncate(int fd, off_t length){

	debug("[SHIM][ftruncate]\t");
	print_info_fd(fd);
	//debug("ftruncate call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "length", length);
	
	return real_ftruncate(fd, length);
}


int truncate64(const char *path, off_t length){

	debug("[SHIM][truncate64] %s\n", path);
	//debug("truncate64 call not handled\n");
	
	// Update statistics
	inc_file_op_filename(path, __func__, "calls", 1);
	inc_file_op_filename(path, __func__, "length", length);

	return real_truncate64(path, length);
}


int ftruncate64(int fd, off_t length){

	debug("[SHIM][ftruncate64]\t");
	print_info_fd(fd);
	//debug("ftruncate64 call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "length", length); 

	return real_ftruncate64(fd, length);
}


int fallocate(int fd, int mode, off_t offset, off_t len){

	debug("[SHIM][fallocate]\t");
	print_info_fd(fd);
	//debug("fallocate call not handled\n");

	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "length", len); 

	return real_fallocate(fd, mode, offset, len);
}


int creat(const char *pathname, mode_t mode){
	int ret;
	
	debug("[SHIM][creat] %s\n", pathname);
	//debug("creat call not handled\n");
	
	ret = real_creat(pathname, mode);

	// Update statistics
	inc_file_op_filename(pathname, __func__, "calls", 1); // increment by 1

	return ret;
}


int posix_fallocate(int fd, off_t offset, off_t len){

	debug("[SHIM][posix_fallocate]\t");
	print_info_fd(fd);
	//debug("posix_fallocate call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 
	inc_file_op_fd(fd, __func__, "length", len); 

	return real_posix_fallocate(fd, offset, len);
}


int sync_file_range(int fd, off64_t offset, off64_t nbytes, unsigned int flags){

	debug("[SHIM][sync_file_range]");
	print_info_fd(fd);
	//debug("sync_file_range call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 
	inc_file_op_fd(fd, __func__, "bytes", nbytes); 
	
	return real_sync_file_range(fd, offset, nbytes, flags);
}


ssize_t readahead(int fd, off64_t offset, size_t count){

	debug("[SHIM][readahead]\t");
	print_info_fd(fd);
	//debug("readahead call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "bytes", count); 
	
	return real_readahead(fd, offset, count);
}


FILE *fopen(const char *path, const char *mode){
	FILE *ret;

	// For some reason, on certain systems, fopen will be called, before the shim can
	// iniate and link on real functions. In this case, call directly for now. 
	// We don't care about what is happening outside of main()
	if (real_fopen == NULL){
		//printf("[SHIM][fopen] Called pre shim-init to open file %s\n", path);
		FILE* (*tmp_fopen)(const char *path, const char *mode) = (FILE* (*)(const char *, const char *)) dlsym(RTLD_NEXT, "fopen");
		return tmp_fopen(path, mode);
	}

	debug("[SHIM][fopen] \t\t\t\t\t\t%s\n", path);
	//debug("fopen call not handled\n");
	
	ret = real_fopen(path, mode);

	// Update statistics
	inc_file_op_filename(path, __func__, "calls", 1);

	return ret;
}


FILE *fdopen(int fd, const char *mode){

	debug("[SHIM][fdopen]\t");
	print_info_fd(fd);
	//debug("fdopen call not handled\n");

	// No need to check to ignore here, fd already exists and is open
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 
	
	return real_fdopen(fd, mode);
}


/* The freopen() function opens the file whose name is the string pointed to by path and associates the stream pointed to by stream with it. The original stream (if it exists) is closed. The mode argument is used just as in the fopen() function. The primary use of the freopen() function is to change the file associated with a standard text stream (stderr, stdin, or stdout).

If the file is successfully reopened, the function returns the pointer passed as parameter stream, which can be used to identify the reopened stream.
Otherwise, a null pointer is returned.

*/
FILE *freopen(const char *path, const char *mode, FILE *stream){
	FILE *ret;

	debug("[SHIM][freopen] \t\t\t\t\t%s\n", path);
	//debug("freopen call not handled\n");

	ret = real_freopen(path, mode, stream);

	// Update statistics
	inc_file_op_filename(path, __func__, "calls", 1);
	
	return ret;
}


int fclose(FILE *fp){
	

	// For some reason, on certain systems, fclose will be called, before the shim can
	// iniate and link on real functions. In this case, call directly for now. 
	// We don't care about what is happening outside of main()
	if (real_fclose == NULL){
		//printf("[SHIM][fclose] Called pre shim-init\n");
		int (*tmp_fclose)(FILE *fp) = (int (*)(FILE *)) dlsym(RTLD_NEXT, "fclose");
		return tmp_fclose(fp);
	}

	debug("[SHIM][fclose]\t");
	print_info_file(fp);
	//debug("fclose call not handled\n");

	// Update statistics
	inc_file_op_file(fp, __func__, "calls", 1);
	
	return real_fclose(fp);
}


ssize_t read(int fd, void *buf, size_t count){

	debug("[SHIM][read]\t");
	print_info_fd(fd);
	//debug("read call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "bytes", count);

	return real_read(fd, buf, count);
}


ssize_t write(int fd, const void *buf, size_t count){

	debug("[SHIM][write]\t\t");
	print_info_fd(fd);
	//debug("write call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "bytes", count);

	return real_write(fd, buf, count);
}


ssize_t pread(int fd, void *buf, size_t count, off_t offset){

	debug("[SHIM][pread]\t");
	print_info_fd(fd);
	//debug("pread call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "bytes", count);

	return real_pread(fd, buf, count, offset);
}


ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset){

	debug("[SHIM][pwrite]\t");
	print_info_fd(fd);
	//debug("pwrite call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	
	return real_pwrite(fd, buf, count, offset);
}


ssize_t pread64(int fd, void *buf, size_t count, off_t offset){

	debug("[SHIM][pread64]\t");
	print_info_fd(fd);
	//debug("pread64 call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "bytes", count);
	
	return real_pread64(fd, buf, count, offset);
}


ssize_t pwrite64(int fd, const void *buf, size_t count, off_t offset){

	debug("[SHIM][pwrite64]\t");
	print_info_fd(fd);
	//debug("pwrite64 call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);
	inc_file_op_fd(fd, __func__, "bytes", count);

	return real_pwrite64(fd, buf, count, offset);
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){

	debug("[SHIM][fread]\t");
	print_info_file(stream);
	//debug("fread call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);
	inc_file_op_file(stream, __func__, "bytes", size);

	return real_fread(ptr, size, nmemb, stream);
}


size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){

	debug("[SHIM][fwrite]\t");
	print_info_file(stream);
	//debug("fwrite call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);
	inc_file_op_file(stream, __func__, "bytes", size);

	return real_fwrite(ptr, size, nmemb, stream);
}


ssize_t readv(int fd, const struct iovec *iov, int iovcnt){

	debug("[SHIM][readv]\t");
	print_info_fd(fd);
	//debug("readv call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 
	inc_file_op_fd(fd, __func__, "iovcnt", iovcnt);

	return real_readv(fd, iov, iovcnt);
}


ssize_t writev(int fd, const struct iovec *iov, int iovcnt){

	debug("[SHIM][writev]\t");
	print_info_fd(fd);
	//debug("writev call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 
	inc_file_op_fd(fd, __func__, "iovcnt", iovcnt);

	return real_writev(fd, iov, iovcnt);
}


ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset){

	debug("[SHIM][preadv]\t");
	print_info_fd(fd);
	//debug("preadv call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 
	inc_file_op_fd(fd, __func__, "iovcnt", iovcnt);

	return real_preadv(fd, iov, iovcnt, offset);
}


ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset){

	debug("[SHIM][pwritev]\t");
	print_info_fd(fd);
	//debug("pwritev call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1); 
	inc_file_op_fd(fd, __func__, "iovcnt", iovcnt);

	return real_pwritev(fd, iov, iovcnt, offset);
}


off_t lseek(int fd, off_t offset, int whence){

	debug("[SHIM][lseek]\t");
	print_info_fd(fd);
	//debug("lseek call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);

	return real_lseek(fd, offset, whence);
}


int fseek(FILE *stream, long offset, int whence){

	debug("[SHIM][fseek]\t");
	print_info_file(stream);
	//debug("fseek call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_fseek(stream, offset, whence);
}


long ftell(FILE *stream){

	debug("[SHIM][ftell]\t");
	print_info_file(stream);
	//debug("ftell call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_ftell(stream);
}


void rewind(FILE *stream){

	debug("[SHIM][rewind]\t");
	print_info_file(stream);
	//debug("rewind call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);
	
	return real_rewind(stream);
}


int fgetpos(FILE *stream, fpos_t *pos){

	debug("[SHIM][fgetpos]\t");
	print_info_file(stream);
	//debug("fgetpos call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_fgetpos(stream, pos);
}


int fsetpos(FILE *stream, const fpos_t *pos){

	debug("[SHIM][fsetpos]\t");
	print_info_file(stream);
	//debug("fsetpos call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_fsetpos(stream, pos);
}


int fgetc(FILE *stream){

	debug("[SHIM][fgetc]\t\t");
	print_info_file(stream);
	//debug("fgetc call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_fgetc(stream);
}


char *fgets(char *s, int size, FILE *stream){

	debug("[SHIM][fgets]\t\t");
	print_info_file(stream);
	//debug("fgets call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);
	inc_file_op_file(stream, __func__, "bytes", size);

	return real_fgets(s, size, stream);
}


int getc(FILE *stream){

	debug("[SHIM][getc]\t\t");
	print_info_file(stream);
	//debug("getc call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_getc(stream);
}


int ungetc(int c, FILE *stream){

	debug("[SHIM][ungetc]\t");
	print_info_file(stream);
	//debug("ungetc call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_ungetc(c, stream);
}


int fputc(int c, FILE *stream){

	debug("[SHIM][fputc]\t\t");
	print_info_file(stream);
	//debug("fputc call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);
	
	return real_fputc(c, stream);
}


int fputs(const char *s, FILE *stream){

	debug("[SHIM][fputs]\t\t");
	print_info_file(stream);
	//debug("fputs call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_fputs(s, stream);
}


int putc(int c, FILE *stream){

	debug("[SHIM][putc]\t\t");
	print_info_file(stream);
	//debug("putc call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_putc(c, stream);
}


int fsync(int fd){

	debug("[SHIM][fsync]\t\t");
	print_info_fd(fd);
	//debug("fsync call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);

	return real_fsync(fd);
}


int fdatasync(int fd){

	debug("[SHIM][fdatasync]\t");
	print_info_fd(fd);
	//debug("fdatasync call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);

	return real_fdatasync(fd);
}


void sync(void){

	debug("[SHIM][sync]\n");
	//debug("sync call not handled\n");
	
	// TODO: NEED TO HANDLE THIS 

	return real_sync();
}


int syncfs(int fd){

	debug("[SHIM][syncfs]\t");
	print_info_fd(fd);
	//debug("syncfs call not handled\n");
	
	// Update statistics
	inc_file_op_fd(fd, __func__, "calls", 1);

	return real_syncfs(fd);
}


int fflush(FILE *stream){

	debug("[SHIM][fflush]\t");
	print_info_file(stream);
	//debug("fflush call not handled\n");
	
	// Update statistics
	inc_file_op_file(stream, __func__, "calls", 1);

	return real_fflush(stream);
}


int msync(void *addr, size_t length, int flags){

	debug("[SHIM][msync]\n");
	//debug("msync call not handled\n");
	
	// TODO: NEED TO HANDLE THIS 

	return real_msync(addr, length, flags);
}


/*
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset){

	// NOTE: DO NOT PRINT IN MMAP, there will be issues on locks
	
	// DLSYM ALSO LOCKS??
	
	// But then seg faults because not there?

	// HOW DO WE CALL REAL MMAP WITHOUT it interfering with printf(), dlsym(), etc...??
	// - setting constructor priority to try and have it link before hand does not work

	// For some reason, on certain systems, fopen will be called, before the shim can
	// iniate and link on real functions. In this case, call directly for now. 
	// We don't care about what is happening outside of main()
	if (real_mmap == NULL){
		//printf("[SHIM][mmap] Called pre shim-init\n");
		void* (*tmp_mmap)(void *addr, size_t length, int prot, int flags, int fd, off_t offset) = dlsym(RTLD_NEXT, "mmap");
		return tmp_mmap(addr, length, prot, flags, fd, offset);
	}

	//printf("MMAP\n");
	//debug("[SHIM][mmap]\t");
	//print_info_fd(fd);
	//debug("mmap call not handled\n");
	//return NULL;

	return real_mmap(addr, length, prot, flags, fd, offset);
}
*/


void *mmap2(void *addr, size_t length, int prot, int flags, int fd, off_t pgoffset){

	debug("[SHIM][mmap2]\t");
	print_info_fd(fd);
	//debug("mmap2 call not handled\n");
	
	// TODO: NEED TO HANDLE THIS 

	return real_mmap2(addr, length, prot, flags, fd, pgoffset);
}


/*
int munmap(void *addr, size_t length){

	debug("[SHIM][munmap]\n");
	//debug("munmap call not handled\n");

	return real_munmap(addr, length);
}
*/


char* mmap_private(const char* filename, size_t* filesize){

	debug("[SHIM][mmap_private] %s\n", filename);
	//debug("mmap_private call not handled\n");
	
	// Update statistics
	inc_file_op_filename(filename, __func__, "calls", 1);

	return real_mmap_private(filename, filesize);
}


char* mmap_read(const char* filename, size_t* filesize){

	debug("[SHIM][mmap_read] %s\n", filename);
	//debug("mmap_read call not handled\n");

	// Update statistics
	inc_file_op_filename(filename, __func__, "calls", 1);

	return real_mmap_read(filename, filesize);
}


char* mmap_shared(const char* filename,size_t* filesize){

	debug("[SHIM][mmap_shared] %s\n", filename);
	//debug("mmap_shared call not handled\n");
	
	// Update statistics
	inc_file_op_filename(filename, __func__, "calls", 1);

	return real_mmap_shared(filename, filesize);
}
