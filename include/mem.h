#ifndef __MEM_MANAGER_H__
#define  __MEM_MANAGER_H__

#include "conf.h"

typedef struct XMEM{
	size_t size;
	size_t free;
	struct XMEM *next;
	char *mem;
} XMEM;

XMEM* init_block(size_t size);
void couple_block(XMEM *pool, XMEM *block);
XMEM *last_block(XMEM *pool);

void gxfree(XMEM *block);

char *xmalloc(XMEM *pxmem, size_t size);
char *xstrdup(XMEM *pxmem, const char *str);




#endif
