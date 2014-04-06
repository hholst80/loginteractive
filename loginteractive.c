#include <stdio.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#define __USE_GNU
#include <dlfcn.h>

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>

static ssize_t (*real_read)(int,void*,size_t) = NULL;

static int stdindata = 0;

static void rwlog_init(void)
{
	real_read = dlsym(RTLD_NEXT, "read");
	if (NULL == real_read) {
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	}
	stdindata = open("stdin.txt",O_RDONLY);
	if (stdindata < 0) {
		perror("open");
	}
}

ssize_t read(int fd, char *buffer, size_t length)
{
	ssize_t count = 0;
	if (real_read==NULL) {
		rwlog_init();
	}
	if (fd!=0) {
		return real_read(fd,buffer,length);
	}
	while (count < length) {
		int rc = real_read(stdindata,buffer+count,1);
		if (rc == 0)
			break;
		++count;
		if (buffer[count] == '\n')
			break;
	}
	return count;
}
