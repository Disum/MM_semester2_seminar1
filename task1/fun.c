#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fun.h"
#define MAX_LEN 256

/* Ф-ия выводит в name_out строки из name_in, заменяя каждое вхождение s на r.
 * Ф-ия возвращает:
 * -1, если не удалось открыть name_in;
 * -2, если не удалось открыть name_out;
 * -3, если не удалось выделиь память;
 * кол-во измененных строк, если проблем не возникло. */
int fun(const char *name_in, const char *name_out, const char *s, const char *r)
{
	FILE *in, *out;
	int count = 0;
	char *buff, *c;

	if( !(in = fopen(name_in, "r")) )
		return -1;
	if( !(out = fopen(name_out, "w")) )
	{
		fclose(in);
		return -2;
	}

	if( !(buff = (char *)malloc(MAX_LEN)) )
	{
		fclose(in);
		fclose(out);
		return -3;
	}

	for( c = (char *)s; *c!='\0' && *c!='\n'; c++ ) { }
	*c = '\0';

	while( fgets(buff, MAX_LEN, in) )
	{
		for( c = buff; *c!='\0' && *c!='\n'; c++ ) { }
		*c = '\0';

		if( !strcmp(buff, s) )
		{
			count++;
			fprintf(out, "%s\n", r);
		} else
		{
			fprintf(out, "%s\n", buff);
		}
	}

	free(buff);
	fclose(in);
	fclose(out);
	return count;
}
