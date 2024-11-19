#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "mem.h"
#include "conf.h"
#include "functions.h"

char *default_name = DEF_SECTION_NAME;
char *separator_char =  DEF_SEPARATOR_CHAR;
char *comment_char = DEF_COMMENT_CHAR;
/* extern char *comment_char; */
char *start_section_char = DEF_START_SECTION_CHAR;
char *stop_section_char = DEF_STOP_SECTION_CHAR;

extern Conf* conf;
extern char *config_file;
extern char endline;
extern char tab;
extern char space;

#ifdef GLOBAL_NAME_SEC
	char *default_name = GLOBAL_NAME_SEC;
#endif

#ifdef SEPARATOR_CHAR
	char *separator_char = SEPARATOR_CHAR;
#endif

#ifdef COMMENT_CHAR
	char *comment_char = COMMENT_CHAR;
#endif

#ifdef START_SECTION_CHAR
	char *start_section_char = START_SECTION_CHAR;
#endif

#ifdef STOP_SECTION_CHAR
	char *stop_section_char = STOP_SECTION_CHAR;
#endif

//*****************************************************
size_t calc_mem(FILE *fp){
	size_t size_mem = 0;
	size_t size_buf = SIZE_BUF; // Буфер для чтения

	char *buf = (char*)malloc(size_buf + 1); // для чтения fread
	if(!buf)
		return 0;

	while(!(readline(&buf, &size_buf, fp))) {
		char *name = NULL;
	    char *val = NULL;

		int mode = splitline(buf, &name, &val);
		switch (mode){
			case COMMENT_LINE: // комент
			case EMPTY_LINE: // пустая строка
				break;
			case SECTION_LINE:
				// S E C T I O N
				size_mem += sizeof(Section) + strlen(name) + 1;
				break;
			case OPTION_LINE:
				// O P T I O N S
				if(strcmp(name, "Include") != 0){
					size_mem += sizeof(Item) + strlen(name) + 1 + strlen(val) + 1;
				}
				break;
		}
	}
	free(buf);
	if(!feof(fp))
		return 0;

	fseek(fp, 0L, SEEK_SET);
	return size_mem;
}

//*****************************************************
Conf *init_conf(char *namefile, int fl_create_default){
	Conf *c = NULL;
	XMEM *xm = NULL;

	FILE *fp = open_file(namefile);
	if(!(fp))
		return NULL;

	// init pool
	size_t size_mem = calc_mem(fp);
	size_mem += sizeof(Conf);
	if(fl_create_default)
		size_mem += strlen(default_name) + 1 + sizeof(Section); // default section

	if(!(xm = init_block(size_mem))){
		fprintf(stderr, "Error init mem\n");
		return NULL;
	}
	c = (Conf*)xmalloc(xm, sizeof(Conf));
	c->pool = xm;
	c->fp = fp;
	c->g_sec = NULL;

	return c;
}

//*****************************************************
FILE* open_file(const char *namefile){
	FILE *fp = NULL;
	if(!namefile) {
		fprintf(stderr, "Error open config file (no name)\n");
		return NULL;
	}

	if(! (fp = fopen(namefile, "r"))) {
		char *err = strerror(errno);
		fprintf(stderr, "Error open config file settings: %s\n", namefile);
		fprintf(stderr, "%s", err);
		return NULL;
	}
	return fp;
}

//*****************************************************
int readline(char **buf, size_t *sizebuf, FILE *fd){
	// Читаем строку в буфер из файла.
	// Если строка длинная, увеличиваем размер буфера
	size_t used_buf;
	if(!fgets(*buf, *sizebuf, fd))
		return 1; // Error
		// Если дляинна прочитанной стоки равна размеру буфера - \0,
		// увеличиваем буфер и дочитываем строку
	while((used_buf = strlen(*buf)) == (--(*sizebuf)))
	{
		if((*sizebuf + SIZE_BUF) > MAX_SIZE_BUF){
			perror("achieved MAX_SIZE_BUF: ");
			return 1; // Error
		}
		*buf = (char*)realloc(*buf, *sizebuf += SIZE_BUF);
#ifdef Debug
		printf("realoc buffer +%ld\n", *sizebuf);
#endif
		if(!(*buf)){
			perror("error realoc buffer:");
			return 1;
		}
		if(!fgets(*buf + used_buf, SIZE_BUF, fd))
			continue;
	}
	return 0; // Ok
}

//*****************************************************
/*
  Или
  	name - имя секции,
  	value - не имеет значения =NULL
  Или
    name - имя опции,
	value - значение опции
 */

int splitline(char *buf, char **name, char **value){
	char *val = NULL;
	char *trimbuf = trim(buf);
	if( ! strlen(trimbuf) ) return EMPTY_LINE; // Empty

	char *pos_dies = strchr(trimbuf, comment_char[0]);
	if(pos_dies){		// Любой текст после #, игнорируем
		pos_dies[0] = '\0';
		if( ! strlen(trimbuf) )
			return COMMENT_LINE; // Comment
	}

	char *pos_sec = strchr(trimbuf, start_section_char[0]);
	char *pos_end_sec = strchr(trimbuf, stop_section_char[0]);

	if(( (!pos_dies) && pos_sec && pos_end_sec && (pos_sec < pos_end_sec)) || // либо НЕ определен # и позиции секции
		(pos_dies
		&& pos_sec
		&& pos_end_sec
		&& (pos_dies > pos_end_sec) // либо Определен # и позиции секции до #
		&& (pos_sec < pos_end_sec))) // И скобки секции [] расставлены правильно
	{
		// S E C T I O N
		pos_end_sec[0] = '\0';
		pos_sec = trim(pos_sec + 1);
		if(strlen(pos_sec)){
			*name = pos_sec;
			return SECTION_LINE; // Section
		}else{
			return ERROR_LINE;
		}
	}
	val = strchr(trimbuf, separator_char[0]);
	if(val){
		val[0] = '\0';
		*value = trim(val + 1);
	}
	*name = trim(trimbuf);

	return OPTION_LINE;
}

//*****************************************************
Section *create_default_sec(Conf *c){
	Section *rez = NULL;

	rez = (Section*)xmalloc(c->pool, sizeof(Section));
	if(!rez){
		fprintf(stderr, "Error create DEFAULT section\n");
		return rez;
	}
	rez->name = (char*)xstrdup(c->pool, default_name);

#ifdef Debug
	fprintf(stderr, "Create default section %ld + %ld\n", sizeof(Section), strlen(default_name));
#endif

	rez->next = NULL;
	rez->itemlist = NULL;
	c->g_sec = rez;
	return rez;
}

//*****************************************************
Section * case_section(Conf *c, const char *name, const char *val){
	Section *lastsection = NULL;
	Section *newsec = find_section(c, name);

	if(!newsec){ // Секция не найдена, создаем новую
		newsec = (Section*)xmalloc(c->pool, sizeof(Section));
		newsec->name = xstrdup(c->pool, name);

		lastsection = last_sec(c);
		if(lastsection){
			lastsection->next = newsec;
		}else{
			if(c->g_sec)
				c->g_sec->next = newsec;
			else
				c->g_sec = newsec;
		}
	}
	return newsec;
}

int case_option(Conf *c, Section *sec, const char *name, const char *val){
	Item *lastitem = NULL;
	Item *item = find_item(sec, name);
	char *v = NULL;
	if(val)
		v = xstrdup(c->pool, val);

	if(!item){
		item = (Item*)xmalloc(c->pool, sizeof(Item));
		item->name = xstrdup(c->pool, name);
		lastitem = last_item(sec->itemlist);
		if(lastitem){
			lastitem->next = item;
		}else{
			sec->itemlist = item;
		}
	}
	if(val){
		item->value = v;
	}
	return 0;
}
//*****************************************************
Conf* read_conf(char *namef, Conf *prev_conf){
	// Возможен рекурсивный вызов при "Include = имяфайла"

	Conf *c = NULL;
	Section *cur_sec = NULL;
	size_t size_buf = SIZE_BUF; // Буфер для чтения

	if(prev_conf){
		c = init_conf(namef, 0);
		if(!c)
			return NULL;
		cur_sec = prev_conf->g_sec;
	}else{
		c = init_conf(namef, 1);
		if(!c)
			return NULL;
		cur_sec = create_default_sec(c);
	}

	char *buf = (char*)malloc(size_buf + 1); // для чтения fread
	if(!buf) {
		fprintf(stderr, "Error malloc buf for read config\n");
		fclose(c->fp);
		delete_config(c);
		return NULL;
	}

	while(!(readline(&buf, &size_buf, c->fp))) {
		/* item = NULL; */
		char *name = NULL;
	    char *val = NULL;
		/* Item *litem = NULL; */

		int mode = splitline(buf, &name, &val);
		switch (mode){
			case COMMENT_LINE: // комент
			case EMPTY_LINE: // пустая строка
				break;
			case SECTION_LINE:
				// S E C T I O N
				cur_sec = case_section(c, name, val);
				break;
			case OPTION_LINE:
				// I N C L U D E
				if((strcmp(name, "Include") == 0) && val){
					Conf *c2 = read_conf(val, c);
					if(!c2) exit(EXIT_FAILURE);
					couple_block(c->pool, c2->pool);
					break;
				}
				// O P T I O N S
				case_option(c, cur_sec, name, val);
				break;
		}
	}
	free(buf);
	if(!feof(c->fp))
	{
		fprintf(stderr, "Ошибка чтения файла\n");
		fclose(c->fp);
		return NULL;
	}
	fclose(c->fp);
	return c;
}

//*****************************************************
Section *last_sec(Conf *c){
	Section *sec = c->g_sec;
	if(sec){
		while(sec->next)
			sec = sec->next;
		return sec;
	}
	return NULL;
}

//*****************************************************
Item *last_item(Item *item){
	if(item){
		while(item->next)
			item = item->next;
	}
	return item;
}

//*****************************************************
Section* find_section(Conf *conf, const char *namesec){
	Section *cur_sec;

	cur_sec = conf->g_sec;

	while(cur_sec){
		if(cur_sec->name && (strcmp(cur_sec->name, namesec) == 0)){
			return cur_sec;
		}
		cur_sec = cur_sec->next;
	}
	return NULL;
}

//*****************************************************
Item *find_item(const Section *section, const char *nameitem){
	Item *it = section->itemlist;
	while(it){
		if(strcmp(it->name, nameitem) == 0){
			return it;
		}
		it = it->next;
	}
	return it;
}

//*****************************************************
int get_val_as_str(Conf *conf, const char *name_sec, const char *name, char **val){
	Section *sec = find_section(conf, name_sec);
	Item *it = NULL;
	if(sec){
		it = find_item(sec, name);
		if(it){
			*val = it->value;
			return 1;
		}
	}
	return 0;
}

//*****************************************************
int get_val_as_int(Conf *conf, const char *name_sec, const char *name, int *val){
	char *p_val;

	if(get_val_as_str(conf, name_sec, name, &p_val)){
		errno = 0;
		*val = strtol(p_val, NULL, 10);
		if(!errno) return 1;
	}
	return 0;
}

//*****************************************************
int get_val_as_float(Conf *conf, const char *name_sec, const char *name, float *val){
	char *p_val;

	if(get_val_as_str(conf, name_sec, name, &p_val)){
		errno = 0;
		*val = atof(p_val);
		if(!errno) return 1;
	}
	return 0;
}

//*****************************************************
void delete_config(Conf *c){
	if(c){
#ifdef Debug
		printf("delete config\n");
#endif
		gxfree(c->pool);
	}
	c = NULL;
}

//*****************************************************
void print_conf(Conf *c){
	Item *curitem = NULL;

	while(c){
		Section *cursec = c->g_sec;
		while(cursec){
#ifdef Debug
			printf("Section name: %c%s%c\n", *start_section_char, cursec->name, *stop_section_char);
#endif
			curitem = cursec->itemlist;
			while(curitem){
#ifdef Debug
				printf("\t%s = %s\n", curitem->name, curitem->value);
#endif
				curitem = curitem->next;
			}
			cursec = cursec->next;
		}
		if(c->pool->next)
			c = (Conf*)(c->pool->next->mem);
		else
			c = NULL;
	}

}

//*****************************************************
void reread_conf(int sig){

	if(conf){
		delete_config(conf);
		conf = NULL;
	}

#ifdef Debug
	printf("reread config: %s\n", config_file);
#endif
	conf = read_conf(config_file, NULL);
	if(!conf){
#ifdef Debug
		printf("error parse config %s\n", config_file);
#endif
		delete_config(conf);
		exit(EXIT_FAILURE);
	}
	return;
}

