#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "mem.h"
#include "conf.h"

extern char *default_name;
extern char *comment_char;

size_t calc_mem(FILE *fin){
	size_t size_mem = 0;
	int els = 0;
	int all_chars = 0;
	char ch;
    char prch = 0; // prev char
	char zch = 0;
	int n_struct = 0;
	int n_str = 0;

	while((ch = fgetc(fin)) != EOF){
		if(ch == comment_char[0]){
			while((ch = fgetc(fin)) != EOF){
				prch = ch;
				if(ch == '\n'){
					break;
				}
			}
			continue;
		}
        if(ch == '\n'){
			if(prch == 0 || prch == '\n')
			   continue;

			els++;
			zch++;
		}else if(ch == '='){
			zch++;
		}else if(ch != '[' && ch != ']'){
			all_chars++;
		}
		prch = ch;
    }
	n_struct = (els + 1) * sizeof(Item);
	n_str = sizeof(char) * (all_chars + zch + strlen(default_name));
	size_mem = n_struct + n_str;

#ifdef Debug
	printf("calculate mem: n_struct %d = %d\tn_str %d\tall_chars %d\tzch %d\n", els + 1, n_struct, n_str, all_chars, zch);
	printf("total calculate mem: %ld\n", size_mem);
#endif
	return size_mem;
}

int init_mem(size_t size){

	if(size <= 0){
		printf("worng size init\n");
		return 1;
	}
	if(xmem.mem){
		return xrealloc(size);
	}else{
		if(!(xmem.mem = (char*)malloc(sizeof(char) * size))){
			printf("error init mem (alloc) %ld bytes\n", size);
			return 1;
		}
#ifdef Debug
		printf("init mem size: %ld\n", size);
#endif
	}

	memset(xmem.mem + (xmem.size - xmem.free), 0, sizeof(char) * size);
	xmem.free += size;
	xmem.size += size;
#ifdef Debug
	printf("mem free: %ld\tsize: %ld\n", xmem.free, xmem.size);
#endif
	return 0;
}

void xfree(char **ptr){
	/* char *p = *ptr; */
	/* if(!xmem) */
		/* return; */
	if(!xmem.mem)
		return;

	if(xmem.mem == *ptr){
		free(xmem.mem);
		xmem.mem = NULL;
#ifdef Debug
		printf("free global mem\n");
#endif
	}else{
#ifdef Debug
		printf("free internal mem: %s\n", *ptr);
#endif
		*ptr = NULL;
	}
	return;
}

int xrealloc(size_t size){
	size_t newsize = xmem.size + (size - xmem.free);
	xmem.mem = (char*)realloc(xmem.mem, newsize);
	if(errno == ENOMEM){
		perror("ERROR REALLOC xmem:");
		return 1;
	}
	xmem.size = newsize;
	xmem.free = xmem.free + (size - xmem.free);
#ifdef Debug
	printf("realloc\t\t size:%ld\t free:%ld\n", xmem.size, xmem.free);
#endif
	return 0;
}

char *xmalloc(size_t size){
	char *rez = NULL;
	if(!size) return NULL;
	if(xmem.free < size)
		xrealloc(size);

	rez = xmem.mem + (xmem.size - xmem.free);
	xmem.free = xmem.free - size;
#ifdef Debug
	printf("alloc size:%ld\t size:%ld\tfree:%ld\n", size, xmem.size, xmem.free);
#endif
	return rez;
}

char *xstrdup(const char *str){
	if(!str) return NULL;

	char *newstr = xmalloc(strlen(str) + 1);
	strcpy(newstr, str);
#ifdef Debug
	printf("copy: %s\n", newstr);
#endif
	return newstr;
}

