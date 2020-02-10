#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fun.h"
char *strstr_whsp(const char *str1, const char *str2, const int *whsp);
#define MAX_LEN 1024
#define CHAR_SZ 256

/* str1, str2 - нуль-терминированные строки;
 * whsp - массив из CHAR_SZ элементов. */
char *strstr_whsp(const char *str1, const char *str2, const int *whsp)
{
	char *c1, *c1_beg, *c2;

	for( c1_beg = (char *)str1, c1 = c1_beg; *c1_beg!='\0'; c1_beg++, c1 = c1_beg )
	{
		c2 = (char *)str2;
		while( *c1==*c2 )
		{
			do
			{
				c1++;
			}
			while( whsp[(int)( (unsigned char)*c1 )] && *c1!='\0' );

			c2++;

			if( *c2=='\0' )
				return c1_beg;
			if( *c1=='\0' )
				break;
		}
	}

	return NULL;
}

/* Ф-ия выводит в name_out строки из name_in, в которые входит s.
 * Символы, содержащиеся в t, не учитываются.
 * Ф-ия возвращает:
 * -1, если не удалось открыть name_in;
 * -2, если не удалось открыть name_out;
 * -3, если не удалось выделиь память;
 * кол-во выведенных строк, если проблем не возникло. */
int fun(const char *name_in, const char *name_out, const char *s, const char *t)
{
	FILE *in, *out;
	int i, count = 0, *whsp, s_sz;
	char *buff, *s_no_whsp, *c;

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
	if( !(whsp = (int *)malloc(CHAR_SZ*sizeof(int))) )
	{
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}

	for( s_sz = 0; s[s_sz]!='\0' && s[s_sz]!='\n'; s_sz++ ) { }
	if( !(s_no_whsp = (char *)malloc(s_sz)) )
	{
		free(whsp);
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}

	for( i = 0; i<CHAR_SZ; i++ )
	{
		whsp[i] = 0;
	}
	for( c = (char *)t; *c!='\0' && *c!='\n'; c++ )
	{
		whsp[(int)( (unsigned char)*c )] = 1;
	}

	c = (char *)s_no_whsp;
	for( i = 0; i<s_sz; i++ )
	{
		if( !whsp[(int)( (unsigned char)s[i] )] )
		{
			*c = s[i];
			c++;
		}
	}
	*c = '\0';

	while( fgets(buff, MAX_LEN, in) )
	{
		for( c = buff; *c!='\0' && *c!='\n'; c++ ) { }
		*c = '\0';

		if( strstr_whsp(buff, s_no_whsp, whsp) )
		{
			count++;
			fprintf(out, "%s\n", buff);
		}
	}

	free(s_no_whsp);
	free(whsp);
	free(buff);
	fclose(in);
	fclose(out);
	return count;
}
