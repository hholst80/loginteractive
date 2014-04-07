/**
 * @file loginteractive.c
 * @author Henrik Holst <holst@matmech.com>
 * @license LICENSE
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#define __USE_GNU
#include <dlfcn.h>

static ssize_t (*real_read)(int,void*,size_t) = NULL;
static int (*real_select)(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout) = NULL;

static int stdindata = 0;
static int stdindata_eof = 0;

static void init(void)
{
	real_read = dlsym(RTLD_NEXT, "read");
	if (NULL == real_read) {
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	}
	real_select = dlsym(RTLD_NEXT, "select");
	if (NULL == real_select) {
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	}
	if (getenv("STDIN")) {
		stdindata = open(getenv("STDIN"),O_RDONLY);
	} else {
		stdindata = open("stdin.txt",O_RDONLY);
	}
	if (stdindata < 0) {
		perror("open");
	}
}

int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout)
{
	int count = 0;
	if (!real_select) {
		init();
	}
	if (!stdindata_eof) {
		FD_SET(0,readfds);
		return 1;
	}
	return real_select(nfds,readfds,writefds,errorfds,timeout);
}

ssize_t read(int fd, char *buffer, size_t length)
{
	ssize_t count = 0;
	if (real_read==NULL) {
		init();
	}
	if (fd!=0) {
		return real_read(fd,buffer,length);
	}
	while (count < length) {
		int rc = real_read(stdindata,buffer+count,1);
		if (rc == 0) {
			stdindata_eof = 1;
			break;
		}
		++count;
		if (buffer[count] == '\n')
			break;
	}
	return count;
}
