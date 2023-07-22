#ifndef __CONF_H__
#define __CONF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_BUF 512
#define COMMENT_CHAR '#'
#define VALUE_SEPARATOR_CHAR '='
#define START_SECTION_CHAR '['
#define STOP_SECTION_CHAR ']'
#define default_name_sec "GLOBAL"

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
struct _section *default_sec;

int read_file(const char* namefile);
struct _section* find_section(const char *namesec);
struct _item *find_item(const struct _section *section, const char *nameitem);
void delete_config(struct _section *sec);
void print_conf(struct _section *section);

#endif
