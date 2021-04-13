#ifndef _PREDICTOR_HPP
#define _PREDICTOR_HPP

bool handle_open(int fd, const char *filename);
int handle_read(int fd, off_t pos, size_t bytes);
int handle_write(int fd, off_t pos, size_t bytes);
int handle_close(int fd);
#endif

