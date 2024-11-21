#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "mem.h"
#include "conf.h"

#define DEBUG 0

//*****************************************************
XMEM* init_block(size_t size){
	XMEM *block = NULL;
	if(size <= 0){
		fprintf(stderr, "worng size mem init\n");
		return NULL;
	}
	block = (XMEM*)malloc(size + sizeof(XMEM));
	if(!block){
		fprintf(stderr, "Error malloc block size: %ld\n", size);
		return NULL;
	}
	block->size = size;
	block->free = size;
	block->next = NULL;
	block->mem = ((char*)block) + sizeof(XMEM);
#ifdef Debug
	fprintf(stdout, "Инициализируем память %ld + xmem (%ld) = %ld\n", size, sizeof(XMEM), size + sizeof(XMEM));
#endif

	return block;
}

//*****************************************************
void couple_block(XMEM *pool, XMEM *block){
	XMEM *p = pool;
	if(!p){
		fprintf(stderr, "Wrong pool pointer!\n");
		return;
	}

	while(p->next)
		p = p->next;

	if(!block){
		fprintf(stderr, "Wrong block pointer\n");
		return;
	}

	p->next = block;
	return;
}

//*****************************************************
// Освобождает ГЛОБАЛЬНО (не ЛОГИЧЕСКИ)
void gxfree(XMEM *ptr){
	XMEM *next;

	while(ptr){
		next = ptr->next;
#ifdef Debug
		fprintf(stdout, "delete size: %ld, xmem: %ld, ALL delete mem: %ld\n", ptr->size, sizeof(XMEM), ptr->size + sizeof(XMEM));
#endif
		free(ptr);
		ptr = next;
	}
	return;
}

//*****************************************************
char *xmalloc(XMEM *pxmem, size_t size){
	char *rez = NULL;

	if(!pxmem || !(pxmem->mem)){
		fprintf(stderr, "Error allocate mem. Wrong pxmem\n");
		return rez;
	}

	if(size <= 0 || size > pxmem->free){
		fprintf(stderr, "Error allocate mem. requested: %ld bytes, but free only: %ld\n", size, pxmem->free);
		return rez;
	}

	rez = pxmem->mem + (pxmem->size - pxmem->free);
	pxmem->free = pxmem->free - size;
#ifdef Debug
	fprintf(stdout, "alloc size:%ld\t Allsize:%ld\tfree:%ld\n", size, pxmem->size, pxmem->free);
#endif
	return rez;
}

//*****************************************************
char *xstrdup(XMEM *pxmem, const char *str){
	if(!str) return NULL;
	if(!pxmem || !(pxmem->mem)) return NULL;

	char *newstr = xmalloc(pxmem, strlen(str) + 1);
	if(!newstr){
		fprintf(stderr, "Error copy string: size string very big! free: %ld, size string: %ld, str = '%s'\n", pxmem->free, strlen(str) + 1, str);
		exit(EXIT_FAILURE);
	}
	strcpy(newstr, str);
#ifdef Debug
	fprintf(stdout, "copy: %s\n", newstr);
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


