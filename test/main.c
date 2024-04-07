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

Conf* main_conf;

int main(int argc, char* argv[]){
	comment_char = ";";
	/* separator_char = " "; */
	//start_section_char = "[";
	//stop_section_char = "]";

	if(argc < 2){
		printf("USSAGE: %s name_config_file\n", argv[0]);
		return 0;
	}

	main_conf = read_conf(argv[1], NULL);
	if(!main_conf){
		printf("Error read config file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	char* val;

	print_conf(main_conf);
	if(get_val_as_str(main_conf, "Session", "session.serialize_handler", &val)){
		printf("---------\nsession.serialize_handler = %s\n---------\n", val);
	}else{
		printf("FIND ERROR\n");
	}

	delete_config(&main_conf);
	return 0;
}
