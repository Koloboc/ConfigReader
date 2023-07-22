#include "conf.h"
#include "functions.h"

struct _section *sec = NULL;

int read_file(const char* namefile){
	FILE *fp;
	size_t size_buf = SIZE_BUF;
	int count_lines = 0;

	if(!namefile)
	{
		fprintf(stderr, "Error open file settings (no name)\n");
		return -1;
	}

	if(! (fp = fopen(namefile, "r")))
	{
		fprintf(stderr, "Error open file settings: %s\n", namefile);
		return -1;
	}

	char *buf = (char*)malloc(size_buf);
	if(!buf)
	{
		fprintf(stderr, "Error malloc buf\n");
		fclose(fp);
		return -1;
	}

	size_t used_buf = 0;

	sec = (struct _section*)malloc(sizeof(struct _section));
	memset(sec, 0, sizeof(struct _section));
	sec->name = strdup(default_name_sec);

	struct _section* cur_sec = sec;

	while(fgets(buf, size_buf, fp))
	{
		count_lines++;
		// Если дляинна прочитанной стоки равна размеру буфера - \0,
		// увеличиваем буфер и дочитываем строку
		while((used_buf = strlen(buf)) == (--size_buf))
		{
			buf = (char*)realloc(buf, size_buf += SIZE_BUF);
			if(!fgets(buf + used_buf, SIZE_BUF, fp))
				continue;
		}

		char *trimbuf = trim(buf);
		char *pos_dies = strchr(trimbuf, COMMENT_CHAR);
		char *pos_sec = strchr(trimbuf, START_SECTION_CHAR);
		char *pos_end_sec = strchr(trimbuf, STOP_SECTION_CHAR);
		char *pos_eq = strchr(trimbuf, VALUE_SEPARATOR_CHAR);

		if(pos_dies){		// Любой текст после #, игнорируем
		   pos_dies[0] = '\0';
		}
		if( ! strlen(trimbuf) ) continue; 

		// S E C T I O N
		if(( (!pos_dies) && pos_sec && pos_end_sec) || // либо НЕ определен # и позиции секции
			(pos_dies 
			&& pos_sec
			&& pos_end_sec
		    && (pos_dies > pos_sec) 
			&& (pos_dies > pos_end_sec) // либо Определен # и позиции секции до #
			&& (pos_sec < pos_end_sec))) // И скобки секции [] расставлены правильно 
		{
			pos_end_sec[0] = '\0';
			pos_sec = trim(pos_sec + 1);
			if(strlen(pos_sec) < 2){ // Имя секции больше одного символа
				fprintf(stderr, "Error name section '%s' line %d in conf file %s\n", pos_sec + 1, count_lines, namefile);
				continue;
			}

			struct _section *newsec = find_section(pos_sec);
			if(!newsec){ // Секция не найдена, создаем новую
				newsec = (struct _section*)malloc(sizeof(struct _section));
				memset(newsec, 0, sizeof(struct _section));
				newsec->name = strdup(pos_sec);
				cur_sec->next = newsec;
				cur_sec = newsec;
			}
			cur_sec = newsec; // Секция найдена, используем ее
			continue;
		}

		//  E N D    S E C T I O N

		if((pos_eq && !pos_dies) || (pos_eq && pos_dies && (pos_eq < pos_dies))) {
			pos_eq[0] = '\0';
			pos_eq++;
		}else{
			fprintf(stderr, "Error parsing line %d:%s config file %s\n", count_lines, buf, namefile);
			continue;
		}
		if(strlen(trimbuf)){
			struct _item *item = find_item(cur_sec, trimbuf);
			if(!item){
				item = (struct _item*)malloc(sizeof(struct _item)); 
				memset(item, 0, sizeof(struct _item));
				item->name = strdup(trim(trimbuf));
				item->value = strdup(trim(pos_eq));

				struct _item *curitem = cur_sec->itemlist;

				if(curitem){
					while(curitem->next)
						curitem = curitem->next;

					curitem->next = item;
				}else{
					cur_sec->itemlist = item;
				}
			}else{
				if(item->value) free(item->value);
				item->value = strdup(trim(pos_eq));
			}
		}
	}
	free(buf);
	if(!feof(fp))
	{
		fprintf(stderr, "Ошибка чтения файла %s\n", namefile);
		return -1;
	}
	return 0;

}

struct _section* find_section(const char *namesec){
	struct _section *cur_sec = sec;
	while(cur_sec){
		if(cur_sec->name && (strcmp(cur_sec->name, namesec) == 0)){
			return cur_sec;
		}
		cur_sec = cur_sec->next;
	}
	return NULL;
}

struct _item *find_item(const struct _section *section, const char *nameitem){
	struct _item *it = section->itemlist;
	while(it){
		if(strcmp(it->name, nameitem) == 0){
			return it;
		}
		it = it->next;
	}
	return it;
}

void delete_config(struct _section *section){
	struct _item *it = NULL;
	struct _item *temp = NULL;
	struct _section *tempsec = NULL;

	while(section){
		it = section->itemlist;
		while(it){
			//printf("delete name: %s in section: %s\n", it->name, section->name);
			if(it->name) free(it->name);
			//printf("delete value: %s in section: %s\n", it->value, section->name);
			if(it->value) free(it->value);
			temp = it->next;
			free(it);
			it = temp;
		}
		tempsec = section->next;
		//printf("delete name section: %s\n", section->name);
		free(section->name);
		//printf("delete section\n");
		free(section);
		section = tempsec;
	}
}
void print_conf(struct _section *section){
	struct _section *cursec = section;
	struct _item *curitem = NULL;

	while(cursec){
		fprintf(stdout, "section name: '%s'\n", cursec->name);
		curitem = cursec->itemlist;
		while(curitem){
			fprintf(stdout, "param name: '%s', value: '%s'\n", curitem->name, curitem->value);
			curitem = curitem->next;
		}
		cursec = cursec->next;
	}
}

