#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "conf.h"

int init_mem(FILE *fin){
	int endline = 0;
	int all_chars = 0;
	size_t size_mem = 0;
	char ch;
	xmem = NULL;

	while((ch = fgetc(fin)) != EOF){
        if(ch == '\n'){
			endline++;
		}else if(ch == ' '){
			continue;
		}else{
			all_chars++;
		}
    }
	fseek(fin, 0L, SEEK_SET);
	size_mem = endline * sizeof(Item) + sizeof(char) * all_chars;
	/* size_mem = size_mem * 2; */

	if(!(xmem = (XMEM*)malloc(sizeof(XMEM))))
		return 1;

	if(!(xmem->mem = (char*)malloc(sizeof(char) * size_mem)))
		return 1;

	memset(xmem->mem, 0, sizeof(sizeof(char) * size_mem));
	xmem->free = size_mem;
	xmem->size = size_mem;
#ifdef Debug
	printf("Инициализировано %ld bytes\n", size_mem);
#endif
	return 0;
}

void xfree(){
	if(xmem){
		if(xmem->mem){
			free(xmem->mem);
			xmem->mem = NULL;
		}
		free(xmem);
		xmem = NULL;
	}
}

void xrealloc(size_t size){
	size_t newsize = xmem->size + (size - xmem->free);
		xmem->mem = (char*)realloc(xmem->mem, newsize);
		if(!xmem->mem){
			perror("ERROR REALLOC xmem:");
		}
		xmem->size = newsize;
		xmem->free = xmem->free + (size - xmem->free);
#ifdef Debug
		printf("\t\t size:%ld\t free:%ld\n", xmem->size, xmem->free);
#endif
}

char *xmalloc(size_t size){
	char *rez = NULL;
	if(!size) return NULL;
	if(xmem->free < size)
		xrealloc(size);

	rez = xmem->mem + (xmem->size - xmem->free);
	xmem->free = xmem->free - size;
#ifdef Debug
	printf("alloc size:%ld\t size:%ld\tfree:%ld\n", size, xmem->size, xmem->free);
#endif
	return rez;
}

char *xstrdup(const char *str){
	if(!str) return NULL;

	char *newstr = xmalloc(strlen(str) + 1);
	strcpy(newstr, str);
	return newstr;
}

