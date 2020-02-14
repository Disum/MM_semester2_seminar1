#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fun.h"
int strcmp_whsp(const char *str1, const char *str2, const int *whsp);
#define MAX_LEN 1024
#define CHAR_SZ 256

/* str1 и str2 - нуль-терминированные строки;
 * whsp - массив из CHAR_SZ элементов. */
int strcmp_whsp(const char *str1, const char *str2, const int *whsp)
{
	char *c1, *c2;

	c1 = (char *)str1;
	c2 = (char *)str2;
	while( *c1!='\0' || *c2!='\0' )
	{
		while( *c1!='\0' && whsp[(int)( (unsigned char)*c1 )] )
		{
			c1++;
		}

		if( *c1!=*c2 )
			return 1;

		if( *c1!='\0' )
			c1++;
		if( *c2!='\0' )
			c2++;
	}

	return 0;
}

/* Ф-ия выводит в name_out строки из name_in, которые не совпадают с s.
 * Символы, содержащиеся в t, не учитываются при сравнении строк.
 * Ф-ия возвращает:
 * -1, если не удалось открыть name_in;
 * -2, если не удалось открыть name_out;
 * -3, если не удалось выделиь память;
 * кол-во выведенных строк, если проблем не возникло. */
int fun(const char *name_in, const char *name_out, const char *s, const char *t)
{
	FILE *in, *out;
	int i, count = 0, *whsp, s_sz;
	char *buff, *c, *s_no_whsp;

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

	for( s_sz = 0; s[s_sz]!='\0' && s[s_sz]!='\n'; s_sz++ ) { }
	if( !(s_no_whsp = (char *)malloc(s_sz)) )
	{
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}

	if( !(whsp = (int *)malloc(CHAR_SZ*sizeof(int))) )
	{
		free(s_no_whsp);
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}

	for( i = 0; i<CHAR_SZ; i++ ) { whsp[i] = 0; }
	for( c = (char *)t; *c!='\0' && *c!='\n'; c++ )
	{
		whsp[(int)( (unsigned char)*c )] = 1;
	}

	c = s_no_whsp;
	for( i = 0; i<s_sz; i++ )
	{
		if( whsp[(int)( (unsigned char)s[i] )] )
			continue;

		*c = s[i];
		c++;
	}
	*c = '\0';

	while( fgets(buff, MAX_LEN, in) )
	{
		for( c = buff; *c!='\0' && *c!='\n'; c++ ) { }
		*c = '\0';

		if( strcmp_whsp(buff, s_no_whsp, whsp) )
		{
			count++;
			fprintf(out, "%s\n", buff);
		}
	}

	free(whsp);
	free(s_no_whsp);
	free(buff);
	fclose(in);
	fclose(out);
	return count;
}
