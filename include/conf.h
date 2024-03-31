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


#define EMPTY_LINE 0
#define COMMENT_LINE 1
#define SECTION_LINE 2
#define OPTION_LINE 3
#define ERROR_LINE -1

typedef struct _item{
	char *name;
	char *value;
	struct _item *next;
} Item;

typedef struct _section{
	char *name;
	Item *itemlist;
	struct _section * next;
} Section;

FILE* open_file(const char *namefile);
int parse_file(FILE *fp, Section *cur_sec);
int readline(char **buf, size_t *size, FILE *fd);
int splitline(char *buf, char **name, char **value);
FILE* init_conf(const char *namefile);

Section* find_section(const char *namesec);
Item *find_item(const Section *section, const char *nameitem);

void delete_config();
void print_conf();

int get_val_as_str(const char *name_sec, const char *name, char **val);
int get_val_as_int(const char *name_sec, const char *name, int *val);
int get_val_as_float(const char *name_sec, const char *name, double *val);

#endif
