#include "conf.h"

extern struct _section *default_sec;

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("USSAGE: %s name_config_file\n", argv[0]);
		return 0;
	}
 
	if(read_file(argv[1]) != 0){
		fprintf(stderr, "read file error %s\n", argv[1]);
	}
	struct _section *conf = default_sec;
	float *val;

	if(get_val_as_float("SECTION 2", "LR", &val)){
		printf("GLOBAL LR = %f\n", *val);
	}else{
		printf("FIND ERROR\n");
	}

	//print_conf(conf);
	delete_config(conf);
	return 0;
}
