#ifndef _PREDICTOR_HPP
#define _PREDICTOR_HPP

bool handle_open(int fd, const char *filename);
int handle_read(int fd, off_t pos, size_t bytes);
int handle_write(int fd, off_t pos, size_t bytes);
int handle_close(int fd);
bool open_sem();
bool get_sem();
bool post_sem();
bool write_log(int type, pid_t pid, int fd, off_t offset, size_t bytes);
#endif

