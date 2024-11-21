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

//*****************************************************
FILE* open_file(const char *namefile){
	FILE *fp = NULL;
	if(!namefile) {
		fprintf(stderr, "Error open config file (no name)\n");
		return NULL;
	}

	if(! (fp = fopen(namefile, "r"))) {
		char *err = strerror(errno);
		fprintf(stderr, "Error open config file settings: %s\n", namefile);
		fprintf(stderr, "%s", err);
		return NULL;
	}
	return fp;
}


//*****************************************************
int readline(char **buf, size_t *sizebuf, FILE *fd){
	// Читаем строку в буфер из файла.
	// Если строка длинная, увеличиваем размер буфера
	size_t used_buf;
	if(!fgets(*buf, *sizebuf, fd))
		return 1; // Error
		// Если дляинна прочитанной стоки равна размеру буфера - \0,
		// увеличиваем буфер и дочитываем строку
	while((used_buf = strlen(*buf)) == (--(*sizebuf)))
	{
		if((*sizebuf + SIZE_BUF) > MAX_SIZE_BUF){
			perror("achieved MAX_SIZE_BUF: ");
			return 1; // Error
		}
		*buf = (char*)realloc(*buf, *sizebuf += SIZE_BUF);
#ifdef Debug
		fprintf(stdout, "realoc buffer +%ld\n", *sizebuf);
#endif
		if(!(*buf)){
			perror("error realoc buffer:");
			return 1;
		}
		if(!fgets(*buf + used_buf, SIZE_BUF, fd))
			continue;
	}
	return 0; // Ok
}

