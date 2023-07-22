#include <string.h>
#include "functions.h"

char *trim(char *str)
{
	// head
	while(str[0] == ' '
	   || str[0] == '\t'
	   || str[0] == '\n')
		str++;

	// tail
	int len = strlen(str);
	while(len
		  && (str[len -1] == ' '
		  || str[len -1] == '\t'
		  || str[len - 1] == '\n'))
	{
		len--;
		str[len] = '\0';
	}
	return str;
}

