#ifndef __MEM_MANAGER_H__
#define  __MEM_MANAGER_H__

typedef struct XMEM{
	char *mem;
	size_t size;
	size_t free;
} XMEM;
XMEM xmem;

size_t calc_mem(FILE *fin);
int init_mem(size_t size);
void xfree(char **ptr);
char *xmalloc(size_t size);
int xrealloc(size_t size);
char *xstrdup(const char *str);




#endif
