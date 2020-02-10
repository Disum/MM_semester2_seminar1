#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "fun.h"
#define MAX_LEN 1024
#define CHAR_SZ 256

/* Ф-ия выводит в name_out строки из name_in,
 * заменяя каждое вхождение символа из s на соответствующий символ из r.
 * Если символов в s больше, чем в r, недостающие полагаются '\0'.
 * Если в r символов больше, лишние игнорируются.
 * Ф-ия возвращает:
 * -1, если не удалось открыть name_in;
 * -2, если не удалось открыть name_out;
 * -3, если не удалось выделиь память;
 * кол-во замененных символов, если проблем не возникло. */
int fun(const char *name_in, const char *name_out, const char *s, const char *r)
{
	FILE *in, *out;
	int count = 0, i, *check;
	char *buff, *c, *change;

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
	if( !(change = (char *)malloc(CHAR_SZ)) )
	{
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}
	if( !(check = (int *)malloc(CHAR_SZ*sizeof(int))) )
	{
		free(change);
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}

	for( i = 0; i<CHAR_SZ; i++ )
	{
		change[i] = (char)i;
		check[i] = 0;
	}
	for( i = 0; s[i]!='\0' && s[i]!='\n' && r[i]!='\0' && r[i]!='\n'; i++ )
	{
		change[(int)( (unsigned char)s[i] )] = r[i];
		check[(int)( (unsigned char)s[i] )] = 1;
	}
	for( ; s[i]!='\0' && s[i]!='\n'; i++ )
	{
		change[(int)( (unsigned char)s[i] )] = '\0';
		check[(int)( (unsigned char)s[i] )] = 1;
	}

	while( fgets(buff, MAX_LEN, in) )
	{
		for( c = buff; *c!='\0' && *c!='\n'; c++ )
		{
			if( check[(int)( (unsigned char)*c )] )
				count++;

			fprintf(out, "%c", change[(int)( (unsigned char)*c )]);
		}
		fprintf(out, "\n");
	}

	free(check);
	free(change);
	free(buff);
	fclose(in);
	fclose(out);
	return count;
}
