#include <errno.h>
#include "conf.h"
#include "functions.h"
#include "mem.h"

Section *default_sec = NULL;

#ifdef GLOBAL_NAME_SEC
	char *default_name = GLOBAL_NAME_SEC;
#else
	char *default_name = DEF_SECTION_NAME;
#endif

#ifdef SEPARATOR_CHAR
	char *separator_char = SEPARATOR_CHAR;
#else
	char *separator_char =  DEF_SEPARATOR_CHAR;
#endif

#ifdef COMMENT_CHAR
	char *comment_char = COMMENT_CHAR;
#else
	char *comment_char = DEF_COMMENT_CHAR;
#endif

#ifdef START_SECTION_CHAR
	char *start_section_char = START_SECTION_CHAR;
#else
	char *start_section_char = DEF_START_SECTION_CHAR;
#endif

#ifdef STOP_SECTION_CHAR
	char *stop_section_char = STOP_SECTION_CHAR;
#else
	char *stop_section_char = DEF_STOP_SECTION_CHAR;
#endif

extern XMEM *xmem;
	
Section* init_conf(const char *namefile){
	FILE *fp = open_file(namefile);

	if(!fp)
		return NULL;
	// init xmem
	if(init_mem(fp)){
		perror("Error init mem");
		return NULL;
	}

	default_sec = (Section*)xmalloc(sizeof(Section));
	default_sec->name = xstrdup(default_name);
	 
	parse_file(fp, NULL);
	return default_sec;
}

FILE* open_file(const char *namefile)
{
	FILE *fp = NULL;
	if(!namefile)
	{
		fprintf(stderr, "Error open config file (no name)\n");
		return NULL;
	}

	if(! (fp = fopen(namefile, "r")))
	{
		fprintf(stderr, "Error open config file settings: %s\n", namefile);
		return NULL;
	}
	return fp;
}

int readline(char **buf, size_t *sizebuf, FILE *fd)
{
	// Читаем строку в буфер из файла.
	// Если строка длинная, увеличиваем размер буфера
	size_t used_buf;
	if(!fgets(*buf, *sizebuf, fd))
		return 0; // Error
		// Если дляинна прочитанной стоки равна размеру буфера - \0,
		// увеличиваем буфер и дочитываем строку
	while((used_buf = strlen(*buf)) == (--(*sizebuf)))
	{
		if((*sizebuf + SIZE_BUF) > MAX_SIZE_BUF){
			perror("achieved MAX_SIZE_BUF: ");
			return 0; // Error
		}
		*buf = (char*)realloc(*buf, *sizebuf += SIZE_BUF);
		printf("realoc buffer +%ld\n", *sizebuf);
		if(!(*buf)){
			perror("error realoc buffer:");
			return 0;
		}
		if(!fgets(*buf + used_buf, SIZE_BUF, fd))
			continue;
	}
	return 1; // Ok
}

/*
  Или 
  	name - имя секции, 
  	value - не имеет значения =NULL
  Или 
    name - имя опции, 
	value - значение опции
  Возврат: 0 - пустая строка
  		   1 - коммент
		   2 - секция
  		   3 - опция
		  -1 - ошибка
 */
int splitline(char *buf, char **name, char **value)
{
	char *val = NULL;
	char *trimbuf = trim(buf);
	if( ! strlen(trimbuf) ) return 0; // Empty

	char *pos_dies = strchr(trimbuf, comment_char[0]);
	if(pos_dies){		// Любой текст после #, игнорируем
		pos_dies[0] = '\0';
		if( ! strlen(trimbuf) ) return 1; // Comment
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
			return 2; // Section
		}else{
			return -1;
		}
	}
	val = strchr(trimbuf, separator_char[0]);
	if(val){
		val[0] = '\0';
		*value = trim(val + 1);
	}
	*name = trim(trimbuf);

	return 3; 
}

int parse_file(FILE* fp, Section *cur_sec)
{
	// Возможен рекурсивный вызов при "Include = имяфайла"

	size_t size_buf = SIZE_BUF;
	int count_lines = 0;
	
	char *buf = (char*)malloc(size_buf);
	if(!buf)
	{
		fprintf(stderr, "Error malloc buf for config\n");
		fclose(fp);
		xfree();
		return -1;
	}

	if(!cur_sec)
		cur_sec = default_sec;

	while(readline(&buf, &size_buf, fp))
	{
		count_lines++;
		char *name = NULL;
	    char *val = NULL;
		Section *newsec;
		Section *last_sec;
		Item *item;
		int mode = splitline(buf, &name, &val);
		switch (mode){
			case 0:
			case 1:
				// пустая строка
				break;
			case 2:
				// S E C T I O N
				newsec =  find_section(name);
				if(!newsec){ // Секция не найдена, создаем новую
					newsec = (Section*)xmalloc(sizeof(Section));
					newsec->name = xstrdup(name);
					last_sec = cur_sec;
					while(last_sec->next) 
						last_sec = last_sec->next;

					last_sec->next = newsec;
				}
				cur_sec = newsec; // Секция найдена, используем ее
				break;
			case 3:
				// O P T I O N S
				if(strcmp(name, "Include") == 0){
					/* Section *prev_sec = cur_sec; */
					FILE *fp2 = open_file(val);

					if(!fp)
						fprintf(stderr, "Ошибка чтения файла %s\n", val);

					if(parse_file(fp2, cur_sec) == -1){
						fprintf(stderr, "Ошибк  разбора файла %s\n", val);
					}
					/* cur_sec = prev_sec; */
					break;
				}

				item = find_item(cur_sec, name);
				if(!item && name){
					item = (Item*)xmalloc(sizeof(Item)); 
					/* memset(item, 0, sizeof(Item)); */
					item->name = xstrdup(name);
					if(val) item->value = xstrdup(val);

					Item *curitem = cur_sec->itemlist;

					if(curitem){
						while(curitem->next)
							curitem = curitem->next;

						curitem->next = item;
					}else{
						cur_sec->itemlist = item;
					}
				}else{
					if(item->value) free(item->value);
					if(val) item->value = xstrdup(val);
				}
				break;
		}
	}
	free(buf);
	//xfree();
	if(!feof(fp))
	{
		fprintf(stderr, "Ошибка чтения файла\n");
		return -1;
	}
	return 0;

}

Section* find_section(const char *namesec){
	Section *cur_sec = default_sec;
	while(cur_sec){
		if(cur_sec->name && (strcmp(cur_sec->name, namesec) == 0)){
			return cur_sec;
		}
		cur_sec = cur_sec->next;
	}
	return NULL;
}

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

int get_val_as_str(const char *name_sec, const char *name, char **val){
	Section *sec = find_section(name_sec);
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

int get_val_as_int(const char *name_sec, const char *name, int **val){
	char *c_val;

	if(get_val_as_str(name_sec, name, &c_val)){
		errno = 0;
		**val = strtol(c_val, NULL, 10);
		if(!errno) return 1;
	}
	return 0;
}

int get_val_as_float(const char *name_sec, const char *name, float **val){
	char *c_val;

	if(get_val_as_str(name_sec, name, &c_val)){
		errno = 0;
		**val = atof(c_val);
		if(!errno) return 1;
	}
	return 0;
}

void delete_config(){
	xfree();
}

void print_conf(){
	Section *cursec = (Section*)xmem->mem;
	Item *curitem = NULL;

	while(cursec){
		fprintf(stdout, "Section name: %c%s%c\n", *start_section_char, cursec->name, *stop_section_char);
		curitem = cursec->itemlist;
		while(curitem){
			fprintf(stdout, "\t%s = %s\n", curitem->name, curitem->value);
			curitem = curitem->next;
		}
		cursec = cursec->next;
	}
}

