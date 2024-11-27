#ifndef __CONF_H__
#define __CONF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"

typedef struct Item{
	char 		*name;
	char 		*value;
	struct Item *next;
} Item;

typedef struct Section{
	char		   *name;
	Item		   *itemlist;
	struct Section *next;
} Section;

typedef struct _Conf{
	struct XMEM *pool; // Обязательно Первый в структуре
	struct Section *g_sec;
	FILE *fp;
} Conf;

// PUBLIC
Conf* read_conf(char *namefile, Conf *prev_conf);

// INTERNAL
int splitline(char *buf, char **name, char **value);
Conf *init_conf(char *namefile, char **buf, size_t **szbuf, int fl_create_default);
size_t calc_mem(FILE *fin, char **buf, size_t **sz);
Item *last_item(Item *item);
Section *last_sec(Conf *c);

Section * case_section(Conf *c, const char *name);
int case_option(Conf *c, Section *sec, const char *name, const char *val);

Section* find_section(Conf *conf, const char *namesec);
Item *find_item(const Section *section, const char *nameitem);

void delete_config(Conf *c);
void print_conf(Conf *c);

int get_val_as_str(Conf *conf, const char *name_sec, const char *name, char **val);
int get_val_as_int(Conf *conf, const char *name_sec, const char *name, int *val);
int get_val_as_float(Conf *conf, const char *name_sec, const char *name, float *val);
int get_val_as_double(Conf *conf, const char *name_sec, const char *name, double *val);

#endif
