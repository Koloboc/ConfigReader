#include "conf.h"

//extern struct _section *default_sec;
extern char *separator_char;
extern char *comment_char;
extern char *start_section_char;
extern char *stop_section_char;
extern char *separator_char;

int main(int argc, char* argv[]){

	separator_char = "=";
	start_section_char = "[";
	stop_section_char = "]";

	if(argc < 2){
		printf("USSAGE: %s name_config_file\n", argv[0]);
		return 0;
	}
 
	struct _section* conf = init_conf(argv[1]);
	float *val;

	if(get_val_as_float("SECTION 2", "LR", &val)){
		printf("TEST\n LR = %f\n---------\n", *val);
	}else{
		printf("FIND ERROR\n");
	}

	print_conf(conf);
	delete_config(conf);
	return 0;
}
