#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fun.h"
#define MAX_LEN 1024

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
	int count = 0, s_sz, new_s_sz, buff_sz, i;
	char *buff, *c, *new_s;

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

	if( !(new_s = (char *)malloc(s_sz)) )
	{
		free(buff);
		fclose(in);
		fclose(out);
		return -3;
	}

	new_s_sz = 0;
	if( s[s_sz - 1]=='$' )
	{
		for( i = 0; i<(s_sz - 1); i++ )
		{
			if( s[i]=='\\' && (i + 1)<s_sz )
			{
				switch( s[i + 1] )
				{
					case '$':
						new_s[new_s_sz] = '$';
						new_s_sz++;
						i++;
						continue;
					case '\\':
						new_s[new_s_sz] = '\\';
						new_s_sz++;
						i++;
						continue;
				}
			}

			new_s[new_s_sz] = s[i];
			new_s_sz++;
		}
		new_s[new_s_sz] = '\0';

		while( fgets(buff, MAX_LEN, in) )
		{
			for( c = buff; *c!='\0' && *c!='\n'; c++ ) { }
			*c = '\0';

			buff_sz = strlen(buff);
			if( !strncmp(buff + buff_sz - new_s_sz, new_s, new_s_sz) )
			{
				count++;
				fprintf(out, "%s\n", buff);
			}
		}

	}
	else
	{
		for( i = 0; i<s_sz; i++ )
		{
			if( s[i]=='\\' && (i + 1)<s_sz )
			{
				switch( s[i + 1] )
				{
					case '$':
						new_s[new_s_sz] = '$';
						new_s_sz++;
						i++;
						continue;
					case '\\':
						new_s[new_s_sz] = '\\';
						new_s_sz++;
						i++;
						continue;
				}
			}

			new_s[new_s_sz] = s[i];
			new_s_sz++;
		}
		new_s[new_s_sz] = '\0';

		while( fgets(buff, MAX_LEN, in) )
		{
			for( c = buff; *c!='\0' && *c!='\n'; c++ ) { }
			*c = '\0';

			if( strstr(buff, new_s) )
			{
				count++;
				fprintf(out, "%s\n", buff);
			}
		}
	}

	free(new_s);
	free(buff);
	fclose(in);
	fclose(out);
	return count;
}
