#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "mem.h"
#include "conf.h"


//*****************************************************
XMEM* init_block(size_t size){
	XMEM *block = NULL;
	if(size <= 0){
		printf("worng size mem init\n");
		return NULL;
	}
	block = (XMEM*)malloc(size + sizeof(XMEM));
	if(!block){
		printf("Error malloc block size: %ld\n", size);
		return NULL;
	}
	block->size = size;
	block->free = size;
	block->mem = ((char*)block) + sizeof(XMEM);

	return block;
}

//*****************************************************
void couple_block(XMEM *pool, XMEM *block){
	XMEM *p = pool;
	if(!p){
		printf("Wrong pool pointer!\n");
		return;
	}

	while(p->next)
		p = p->next;

	if(!block){
		printf("Wrong block pointer\n");
		return;
	}

	p->next = block;
	return;
}

//*****************************************************
// Освобождает ГЛОБАЛЬНО (не ЛОГИЧЕСКИ)
void gxfree(XMEM **ptr){
	XMEM *p = *ptr;
	XMEM *next;

	if(!(p))
		return;

	while(p){
		next = p->next;
		printf("delete xmem size: %ld\n", p->size);
		free(p);
		p = next;
	}
#ifdef Debug
	printf("free global mem\n");
#endif
	return;
}

//*****************************************************
char *xmalloc(XMEM *pxmem, size_t size){
	char *rez = NULL;

	if(!pxmem || !(pxmem->mem)){
		printf("Error allocate mem. Wrong pxmem\n");
		return rez;
	}

	if(size <= 0 || size > pxmem->free){
		printf("Error allocate mem. requested: %ld bytes, but free only: %ld\n", size, pxmem->free);
		return rez;
	}

	rez = pxmem->mem + (pxmem->size - pxmem->free);
	pxmem->free = pxmem->free - size;
#ifdef Debug
	printf("alloc size:%ld\t size:%ld\tfree:%ld\n", size, pxmem->size, pxmem->free);
#endif
	return rez;
}

//*****************************************************
char *xstrdup(XMEM *pxmem, const char *str){
	if(!str) return NULL;
	if(!pxmem || !(pxmem->mem)) return NULL;

	char *newstr = xmalloc(pxmem, strlen(str) + 1);
	if(!newstr){
		printf("Error copy string: size string very big! free: %ld, size string: %ld\n", pxmem->free, strlen(str) + 1);
		exit(EXIT_FAILURE);
	}
	strcpy(newstr, str);
#ifdef Debug
	printf("copy: %s\n", newstr);
#endif
	return newstr;
}

//*****************************************************
XMEM *last_block(XMEM *pool){
	if(!pool) return NULL;

	while(pool->next)
		pool = pool->next;
	return pool;
}


