#ifndef __CONF_H__
#define __CONF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_BUF 512
#define MAX_SIZE_BUF 3072
#define DEF_COMMENT_CHAR "#"
#define DEF_START_SECTION_CHAR "["
#define DEF_STOP_SECTION_CHAR "]"
#define DEF_SEPARATOR_CHAR "="
#define DEF_SECTION_NAME "GLOBAL"


struct _item{
	char *name;
	char *value;
	struct _item *next;
};

struct _section{
	char *name;
	struct _item *itemlist;
	struct _section * next;
};

FILE* open_file(const char *namefile);
int parse_file(const char *namefile, struct _section *cur_sec);
int readline(char **buf, size_t *size, FILE *fd);
int splitline(char *buf, char **name, char **value);
struct _section* init_conf(const char *namefile);

struct _section* find_section(const char *namesec);
struct _item *find_item(const struct _section *section, const char *nameitem);

void delete_config(struct _section *sec);
void print_conf(struct _section *section);

int get_val_as_str(const char *name_sec, const char *name, char **val);
int get_val_as_int(const char *name_sec, const char *name, int **val);
int get_val_as_float(const char *name_sec, const char *name, float **val);

#endif
