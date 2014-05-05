/**
 * @file libisatty.c
 * @author Henrik Holst <holst@matmech.com>
 * @license LICENSE
 */

void __attribute__((constructor)) init(void)
{
}

void __attribute__((destructor)) shutdown(void)
{
}

int isatty(int fd)
{
	return 1;
}
