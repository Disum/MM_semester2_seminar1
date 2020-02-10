#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fun.h"
char *strstr_spcf(const char *str1, const char *str2);
#define MAX_LEN 1024

/* str1, str2 - нуль-терминированные строки. */
char *strstr_spcf(const char *str1, const char *str2)
{
	char *c1, *c1_beg, *c2;

	if( str2[0]=='^' )
	{
		c1 = (char *)str1;
		c2 = (char *)str2 + 1;

		if( *c2=='\0' )
			return (char *)str1;

		while( *c1==*c2 )
		{
			c1++;
			c2++;

			if( *c2=='\0' )
				return (char *)str1;
			if( *c1=='\0')
				break;
		}

		return NULL;
	}

	for( c1_beg = (char *)str1, c1 = c1_beg; *c1_beg!='\0'; c1_beg++, c1 = c1_beg )
	{
		c2 = (char *)str2;

		if( *c1=='^' || *c1=='\\' )
		{
			if( *c2!='\\' )
				continue;
			c2++;
		}

		while( *c1==*c2 )
		{
			c1++;
			c2++;

			if( *c2=='\0' )
				return c1_beg;
			if( *c1=='\0' )
				break;

			if( *c1=='^' || *c1=='\\' )
			{
				if( *c2!='\\' )
					break;
				c2++;
			}
		}
	}

	return NULL;
}

/* Ф-ия выводит в name_out строки из name_in, в которые входит s.
 * Символ '^' в начале s соответствует началу строки из name_in.
 * Символы '^' и '\' из name_in соответствуют '\^' и '\\' в s.
 * Ф-ия возвращает:
 * -1, если не удалось открыть name_in;
 * -2, если не удалось открыть name_out;
 * -3, если не удалось выделиь память;
 * кол-во измененных строк, если проблем не возникло. */
int fun(const char *name_in, const char *name_out, const char *s)
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

		if( strstr_spcf(buff, s) )
		{
			count++;
			fprintf(out, "%s\n", buff);
		}
	}

	free(buff);
	fclose(in);
	fclose(out);
	return count;
}
