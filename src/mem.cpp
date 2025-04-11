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
		fprintf(stderr, "Error (module config:mem.c) Worng size mem init\n");
		return NULL;
	}
	block = (XMEM*)malloc(size + sizeof(XMEM));
#ifdef DebugMem
	fprintf(stdout, "MALLOC MEM block %ld\n", size + sizeof(XMEM));
#endif
	if(!block){
		fprintf(stderr, "Error (module config:mem.c) malloc block size: %ld\n", size);
		return NULL;
	}
	block->size = size;
	block->free = size;
	block->next = NULL;
	block->mem = ((char*)block) + sizeof(XMEM);
#ifdef DebugMem
	fprintf(stdout, "Module config:mem.c Init memory %ld + xmem (%ld) = %ld\n", size, sizeof(XMEM), size + sizeof(XMEM));
#endif

	return block;
}

//*****************************************************
void couple_block(XMEM *pool, XMEM *block){
	XMEM *p = pool;
	if(!p){
		fprintf(stderr, "Error (module config:mem.c) Wrong pool pointer!\n");
		return;
	}

	while(p->next)
		p = p->next;

	if(!block){
		fprintf(stderr, "Error (module config:mem.c) Wrong block pointer\n");
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

#ifdef DebugMem
		fprintf(stdout, "Module config:mem.c delete size: %ld, xmem: %ld, ALL delete mem: %ld\n", ptr->size, sizeof(XMEM), ptr->size + sizeof(XMEM));
#endif

		free(ptr);

#ifdef DebugMemMem
		fprintf(stdout, "Free ptr (xmem)\n");
#endif

		ptr = next;
	}
	return;
}

//*****************************************************
char *xmalloc(XMEM *pxmem, size_t size){
	char *rez = NULL;

	if(!pxmem || !(pxmem->mem)){
		fprintf(stderr, "Error (module config:mem.c) allocate mem. Wrong pxmem\n");
		return rez;
	}

	if(size <= 0 || size > pxmem->free){
		fprintf(stderr, "Error (module config:mem.c) allocate mem. requested: %ld bytes, but free only: %ld\n", size, pxmem->free);
		return rez;
	}

	rez = pxmem->mem + (pxmem->size - pxmem->free);
	pxmem->free = pxmem->free - size;
#ifdef DebugMem
	fprintf(stdout, "Module config:mem.c alloc size:%ld\t Allsize:%ld\tfree:%ld\n", size, pxmem->size, pxmem->free);
#endif
	return rez;
}

//*****************************************************
char *xstrdup(XMEM *pxmem, const char *str){
	if(!str) return NULL;
	if(!pxmem || !(pxmem->mem)) return NULL;

	char *newstr = xmalloc(pxmem, strlen(str) + 1);
	if(!newstr){
		fprintf(stderr, "Error (module config:mem.c) copy string: size string very big! free: %ld, size string: %ld, str = '%s'\n", pxmem->free, strlen(str) + 1, str);
		return newstr;
	}
	strcpy(newstr, str);
#ifdef DebugMem
	fprintf(stdout, "Module config:mem.c copy: %s\n", newstr);
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


