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
#include <unistd.h>

static ssize_t (*real_read)(int,void*,size_t) = NULL;
static int (*real_select)(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout) = NULL;
static ssize_t (*real_write)(int,const void*,size_t) = NULL;

static int g_stdindata = 0;
static int g_follow = 0;

void __attribute__((constructor)) init(void)
{
	real_read = dlsym(RTLD_NEXT, "read");
	if (!real_read)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	real_select = dlsym(RTLD_NEXT, "select");
	if (!real_select)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	real_write = dlsym(RTLD_NEXT, "write");
	if (!real_write)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	if (getenv("STDIN"))
		g_stdindata = open(getenv("STDIN"),O_RDONLY);
	else
		g_stdindata = open("stdin.txt",O_RDONLY);
	if (g_stdindata < 0)
		perror("open");
	if (getenv("FOLLOW"))
		g_follow = atoi(getenv("FOLLOW"));
}

int select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, fd_set *restrict errorfds, struct timeval *restrict timeout)
{
	if (g_stdindata < 0)
		return real_select(nfds,readfds,writefds,errorfds,timeout);
	FD_SET(0,readfds);
	return 1;
}

ssize_t read(int fd, void *buf, size_t nbytes)
{
	if (g_stdindata < 0 || fd != 0)
		return real_read(fd,buf,nbytes);
	ssize_t count = 0;
	while (count < nbytes) {
		ssize_t rc = real_read(g_stdindata,buf+count,1);
		if (rc == 0) {
			if (!g_follow) {
				close(g_stdindata);
				g_stdindata = -1;
				break;
			} else {
				sleep(1);
				continue;
			}
		}
		if (((char*)buf)[count++] == '\n')
			break;
	}
	return count;
}

#if 0
ssize_t write(int fd, const void *buf, size_t n) 
{ 
	return real_write(fd,buf,n);
} 
#endif
