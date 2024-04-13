#include <stdlib.h>
#include "mem.h"
#include "conf.h"

//extern struct _section *default_sec;

/* extern char *separator_char; */
extern char *comment_char;
/* extern char *start_section_char; */
/* extern char *stop_section_char; */
/* extern char *separator_char; */

/* extern Section *default_sec; */

Conf* conf;
char *config_file;

int main(int argc, char* argv[]){
	comment_char = ";";
	/* separator_char = " "; */
	//start_section_char = "[";
	//stop_section_char = "]";

	if(argc < 2){
		printf("USSAGE: %s name_config_file\n", argv[0]);
		return 0;
	}

	config_file = argv[1];
	conf = read_conf(argv[1], NULL);
	if(!conf){
		printf("Error read config file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	char* val;

	print_conf(conf);
	if(get_val_as_str(conf, "Session", "session.serialize_handler", &val)){
		printf("---------\nsession.serialize_handler = %s\n---------\n", val);
	}else{
		printf("FIND ERROR\n");
	}

	delete_config(conf);
	return 0;
}
