#include <string.h>
#include "conf.h"
#include "functions.h"

char endline = '\n';
char tab = '\t';
char space = ' ';

char *trim(char *str)
{
	// head
	while(str[0] == space
	   || str[0] == tab
	   || str[0] == endline)
		str++;

	// tail
	int len = strlen(str);
	while(len
		  && (str[len -1] == space
		  || str[len -1] == tab
		  || str[len - 1] == endline))
	{
		len--;
		str[len] = '\0';
	}
	return str;
}

void scroll_to_endline(FILE* fin){
	char ch;
	while((ch = fgetc(fin)) != EOF)
		if(ch == '\n')
			break;
}

