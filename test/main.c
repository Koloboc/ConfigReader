#include "conf.h"
#include "mem.h"

//extern struct _section *default_sec;

/* extern char *separator_char; */
extern char *comment_char;
/* extern char *start_section_char; */
/* extern char *stop_section_char; */
/* extern char *separator_char; */

extern Section *default_sec;

int main(int argc, char* argv[]){
	comment_char = ";";
	/* separator_char = " "; */
	//start_section_char = "[";
	//stop_section_char = "]";

	if(argc < 2){
		printf("USSAGE: %s name_config_file\n", argv[0]);
		return 0;
	}

	FILE* fin = init_conf(argv[1]);
	parse_file(fin, NULL);
	Section *conf = default_sec;
	char* val;

	print_conf();
	if(get_val_as_str("Session", "session.serialize_handler", &val)){
		printf("session.serialize_handler = %s\n---------\n", val);
	}else{
		printf("FIND ERROR\n");
	}

	delete_config(conf);
	return 0;
}
