#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fun.h"
int check(const char *str, const int *chr);
#define MAX_LEN 1024
#define CHAR_SZ 256

/* chr - массивы из CHAR_SZ элементов. */
int check(const char *str, const int *chr)
{
	char *c;

	for( c = (char *)str; *c!='\0' && *c!='\n'; c++ )
	{
		if( !chr[(int)( (unsigned char)*c )] )
			return 0;
	}

	return 1;
}

/* Ф-ия выводит в name_out строки из name_in, состоящие только из символов, содержащихся в s.
 * Символы, содержащиеся в t, не учитываются.
 * Ф-ия возвращает:
 * -1, если не удалось открыть name_in;
 * -2, если не удалось открыть name_out;
 * -3, если не удалось выделиь память;
 * кол-во выведенных строк, если проблем не возникло. */
int fun(const char *name_in, const char *name_out, const char *s, const char *t)
{
	FILE *in, *out;
	int i, count = 0, *chr;
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
	if( !(chr = (int *)malloc(CHAR_SZ*sizeof(int))) )
	{
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}

	for( i = 0; i<CHAR_SZ; i++ )
	{
		chr[i] = 0;
	}
	for( c = (char *)s; *c!='\0' && *c!='\n'; c++ )
	{
		chr[(int)( (unsigned char)*c )] = 1;
	}
	for( c = (char *)t; *c!='\0' && *c!='\n'; c++ )
	{
		chr[(int)( (unsigned char)*c )] = 1;
	}

	while( fgets(buff, MAX_LEN, in) )
	{
		if( check(buff, chr) )
		{
			count++;
			fprintf(out, "%s", buff);
		}
	}

	free(chr);
	free(buff);
	fclose(in);
	fclose(out);
	return count;
}
