/**
 * @file loginteractive.c
 * @author Henrik Holst <holst@matmech.com>
 * @license LICENSE
 */

#define _POSIX_SOURCE

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
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
#include <sys/uio.h>

static ssize_t (* real_read)(int,void *,size_t) = NULL;
static ssize_t (* real_write)(int,const void *,size_t) = NULL;
static ssize_t (* real_fwrite)(const void * ptr, size_t size, size_t count, FILE * stream) = NULL;
static ssize_t (* real_fputs)(const void * str, FILE * stream) = NULL;
static ssize_t (* real_fprintf)(const FILE * stream, int flag, const char * format, ...) = NULL;

static int g_stdindata = 0;
static FILE * g_stdoutstream = 0;
static int g_follow = 0;
static FILE * g_forceoutstream = 0;

void __attribute__((constructor)) init(void)
{
	real_read = dlsym(RTLD_NEXT, "read");
	if (!real_read)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	real_fputs = dlsym(RTLD_NEXT, "fputs");
	if (!real_fputs)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	real_fwrite = dlsym(RTLD_NEXT, "fwrite");
	if (!real_fwrite)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	real_write = dlsym(RTLD_NEXT, "write");
	if (!real_write)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	real_fprintf = dlsym(RTLD_NEXT, "__fprintf_chk");
	if (!real_fprintf)
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	if (getenv("STDIN"))
		g_stdindata = open(getenv("STDIN"),O_RDONLY);
	else
		g_stdindata = 0;
	if (g_stdindata < 0)
		perror("open");
	if (getenv("STDOUT"))
		g_stdoutstream = fopen(getenv("STDOUT"),"wb");
	else
		g_stdoutstream = stdout;
	if (!g_stdoutstream)
		perror("fopen");
	if (getenv("NOECHO") && atoi(getenv("NOECHO")) > 0) {
		g_forceoutstream = fopen("/dev/null","wb");
	}
	//if (getenv("NOBUF") && atoi(getenv("NOBUF")) > 0)
	//	setbuf(g_stdoutstream,NULL);
}

void __attribute__((destructor)) shutdown(void)
{
}

int isatty(int fd)
{
	return 1;
}

ssize_t read(int fd, void * buf, size_t nbytes)
{
	char * cbuf = buf;
	ssize_t count = 0;
	int got_eol = 0;
	int rc;
	static int got_eof = 0;

	if (!real_read)
		return 0;
	if (fd != 0)
		return real_read(fd,buf,nbytes);
	if (got_eof)
		return 0;
	if (nbytes > 1)
		nbytes = 1;
	while (count < nbytes && !got_eol) {
		rc = real_read(g_stdindata,buf+count,1);
		if (rc == 0) {
			fputc('\n',g_stdoutstream);
			if (!g_follow) {
				got_eof = 1;
				break;
			} else {
				sleep(1);
				continue;
			}
		}
		if (cbuf[count] == '\177') { /* backspace */
			if (g_stdoutstream != stdout)
				fseek(g_stdoutstream, -1, SEEK_CUR);
		}
		if (cbuf[count] == '\n' || cbuf[count] == '\r') { /* line feed */
			if (g_stdoutstream != stdout)
				fputc('\n',g_stdoutstream);
			got_eol = 1;
		}
		++count;
	}
	return count;
}

int fputs(const char * str, FILE * stream)
{
	if (!real_fputs)
		return 0;
	if (stream != stdout)
		return real_fputs(str,stream);
	if (g_forceoutstream)
		stream = g_forceoutstream;
	if (g_stdoutstream != stdout)
		real_fputs(str,g_stdoutstream);
	return real_fputs(str,stream);
}

ssize_t write(int fd, const void *buffer, size_t nbytes)
{
	FILE * stream = stdout;
	ssize_t count = 0;
	if (!real_write)
		return 0;
	if (fd != 1)
		return real_write(fd,buffer,nbytes);
	/* loop over to a fwrite call */
	return fwrite(buffer,1,nbytes,stream);
}

size_t fwrite(const void * ptr, size_t size, size_t count, FILE * stream)
{
	if (!real_fwrite)
		return 0;
	if (stream != stdout)
		return real_fwrite(ptr,size,count,stream);
	if (g_forceoutstream)
		stream = g_forceoutstream;
	if (g_stdoutstream != stdout)
		real_fwrite(ptr,size,count,g_stdoutstream);
	return real_fwrite(ptr,size,count,stream);
}

int __fprintf_chk(FILE * stream, int flag, const char * format, ...)
{
	int rc;
	va_list args;
	va_start(args,format);
	if (!real_fprintf)
		return 0;
	if (stream != stdout) {
		rc = vfprintf(stream,format,args);
		va_end(args);
		return rc;
	}
	if (g_forceoutstream)
		stream = g_forceoutstream;
	if (g_stdoutstream != stdout)
		vfprintf(g_stdoutstream,format,args);
	rc = vfprintf(stream,format,args);
	va_end(args);
	return rc;
}
