/**
 * @file ansistrip.c
 * @author Henrik Holst <holst@matmech.com>
 * @license LICENSE
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main()
{
	enum
	{
		NORMAL = 0,
		ESC    = 1,
		CSI    = 2,
	} mode = NORMAL;

	const char * allowed_control_codes = "\r\n\t";

	while (!feof(stdin))
	{
		int ch = fgetc(stdin);
		switch (mode)
		{
			case CSI:
			if (64 <= ch && ch <= 126)
				mode = NORMAL;
			break;
			case ESC:
			if (ch == '[')
				mode = CSI;
			else if (64 <= ch && ch <= 95)
				mode = NORMAL;
			break;
			case NORMAL:
			if (ch == 27)
				mode = ESC;
			else if (ch > 0x1f)
				fputc(ch,stdout);
			else if (strchr(allowed_control_codes,ch))
				fputc(ch,stdout);
		}
		fflush(stdout);
	}
	return 0;
}
