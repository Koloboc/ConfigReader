#ifndef __MEM_MANAGER_H__
#define  __MEM_MANAGER_H__

typedef struct XMEM{
	char *mem;
	size_t size;
	size_t free;
} XMEM;
XMEM *xmem;

int init_mem(FILE *fin);
void xfree();
char *xmalloc(size_t size);
void xrealloc(size_t size);
char *xstrdup(const char *str);




#endif
