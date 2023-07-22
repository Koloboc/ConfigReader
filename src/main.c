#include "conf.h"

extern struct _section *sec;

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("USSAGE: %s name_config_file\n", argv[0]);
		return 0;
	}

	if(read_file(argv[1]) == 0){
		fprintf(stdout, "read file ok\n");
	}
	struct _section *conf = sec;
	print_conf(conf);
	delete_config(conf);
	return 0;
}
