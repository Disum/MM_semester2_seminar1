#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "fun.h"
#define MAX_LEN 1024

/* Ф-ия выводит в name_out строки из name_in, заменяя каждое вхождение s на lsr.
 * Ф-ия возвращает:
 * -1, если не удалось открыть name_in;
 * -2, если не удалось открыть name_out;
 * -3, если не удалось выделиь память;
 * кол-во измененных строк, если проблем не обнаружено. */
int fun(const char *name_in, const char *name_out, const char *s, const char *l, const char *r)
{
	FILE *in, *out;
	int count = 0, s_sz, l_sz, r_sz, new_s_sz, len;
	char *str, *new_str, *new_s, *c, *c_new_s, *c_new_str, *c_str_last, *c_str_next;

	if( !(in = fopen(name_in, "r")) )
		return -1;
	if( !(out = fopen(name_out, "w")) )
	{
		fclose(in);
		return -2;
	}

	if( !(str = (char *)malloc(MAX_LEN)) )
	{
		fclose(in);
		fclose(out);
		return -3;
	}

	for( c = (char *)s, s_sz = 0; *c!='\0' && *c!='\n'; c++, s_sz++ ) { }
	*c = '\0';
	for( c = (char *)l, l_sz = 0; *c!='\0' && *c!='\n'; c++, l_sz++ ) { }
	for( c = (char *)r, r_sz = 0; *c!='\0' && *c!='\n'; c++, r_sz++ ) { }
	new_s_sz = s_sz + l_sz + r_sz;

	if( !(new_str = (char *)malloc( ((int)(MAX_LEN/s_sz) + 1)*new_s_sz )) )
	{
		free(str);
		fclose(in);
		fclose(out);
		return -3;
	}

	if( !(new_s = (char *)malloc(new_s_sz)) )
	{
		free(str);
		free(new_str);
		fclose(in);
		fclose(out);
		return -3;
	}

	c_new_s = new_s;
	for( c = (char *)l; *c!='\0' && *c!='\n'; c++ )
	{
		*c_new_s = *c;
		c_new_s++;
	}
	for( c = (char *)s; *c!='\0'; c++ )
	{
		*c_new_s = *c;
		c_new_s++;
	}
	for( c = (char *)r; *c!='\0' && *c!='\n'; c++ )
	{
		*c_new_s = *c;
		c_new_s++;
	}
	c_new_s = '\0';

	while( fgets(str, MAX_LEN, in) )
	{
		for( c = str; *c!='\0' && *c!='\n'; c++ ) { }
		*c = '\0';

		c_str_last = str - s_sz;
		c_new_str = new_str;
		if( (c_str_next = strstr(c_str_last + s_sz, s)) )
		{
			count++;

			do
			{
				len = c_str_next - c_str_last - s_sz;
				if( len>0 )
				{
					strncpy(c_new_str, c_str_last + s_sz,  len);
					c_new_str += len;
				}
				strcpy(c_new_str, new_s);
				c_new_str += new_s_sz;

				c_str_last = c_str_next;
			}
			while( (c_str_next = strstr(c_str_last + 1, s)) );

			strcpy(c_new_str, c_str_last + s_sz);

			fprintf(out, "%s\n", new_str);
		} else
		{
			fprintf(out, "%s\n", str);
		}
	}

	free(new_s);
	free(str);
	free(new_str);
	fclose(in);
	fclose(out);
	return count;
}
