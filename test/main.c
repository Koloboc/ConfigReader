#include <stdlib.h>
#include "mem.h"
#include "conf.h"

char *config_file;

//*****************************************************
/* void reread_conf(int sig){ */

/* 	if(conf) */
/* 		delete_config(conf); */

/* #ifdef Debug */
/* 	fprintf(stdout, "Module config:conf.c REread config: %s\n", config_file); */
/* #endif */
/* 	conf = read_conf(config_file, NULL); */
/* 	if(!conf){ */
/* 		fprintf(stderr, "Error (module config:conf.c) REparse config %s\n", config_file); */
/* 		delete_config(conf); */
/* 		/1* exit(EXIT_FAILURE); *1/ */
/* 	} */
/* 	return; */
/* } */

int main(int argc, char* argv[]){
	Conf *conf;

	if(argc < 4){
		printf("USSAGE: %s <name_config_file> <section_name> <param_name>\n", argv[0]);
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
	if(get_val_as_str(conf, argv[2], argv[3], &val)){
		printf("---------\nsection: [%s] %s = %s\n---------\n", argv[2], argv[3], val);
	}else{
		printf("---------\nFIND ERROR: [%s] %s\n---------\n", argv[2], argv[3]);
	}

	delete_config(conf);
	return 0;
}

