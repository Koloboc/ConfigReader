#ifndef __FUNCTIONS_H__
#define  __FUNCTIONS_H__

#include <stdio.h>

char * trim(char *str);
void scroll_to_endline(FILE* fin);
FILE* open_file(const char *namefile);
int readline(char **buf, size_t *size, FILE *fd);

#endif
