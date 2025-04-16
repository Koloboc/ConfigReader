// #include <stdio.h>
// #include <errno.h>
// #include <unistd.h>
#include <fcntl.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "conf.h"
#include "functions.h"
#include "defines.h"

Storage::Storage(){
}

void Storage::insert(const std::string& name_sec, const std::string &name, const std::string &val){
	std::unordered_map<std::string, Item>::iterator sec;
	sec = stor.find(name_sec);
	if(sec == stor.end()){ // not faund
		Item it{{name, val}};
		stor[name_sec] = it;
	}else{					// faund
		(*sec).second.insert(std::make_pair(name, val));
	}
}

Conf::Conf(){
	str_last_sec = DEF_SECTION_NAME;
}

void Conf::trim(std::string &str){
	auto fi_space = [](unsigned char ch){ return !std::isspace(ch);};
	// trim left
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), fi_space));

	// trim right
	str.erase(std::find_if(str.rbegin(), str.rend(), fi_space).base(), str.end());
}

bool Conf::add_section(const std::string &str_line, size_t pos_start_sec, size_t pos_stop_sec){
	std::string name_sec;
	name_sec = str_line.substr(pos_start_sec + 1, pos_stop_sec - 1);
	trim(name_sec);
	if(!name_sec.length())
		return false;

	str_last_sec = name_sec;
	return true;
}

//*****************************************************
bool Conf::read_conf(char *namef){

	std::string str_line;
	size_t nopos = std::string::npos;

	std::ifstream in(namef);
	if(!in.is_open())
		return false;


	while(std::getline(in, str_line)){
		std::string name;
		std::string val;

		trim(str_line);
		// find and erase #comments
		auto fi_comment = [](unsigned char ch){ return (ch == DEF_COMMENT_CHAR);};
		str_line.erase(std::find_if(str_line.begin(), str_line.end(), fi_comment), str_line.end());
		trim(str_line);

		if(!str_line.length())
			continue;

		size_t pos_start_sec = str_line.find(DEF_START_SECTION_CHAR);
		size_t pos_stop_sec = str_line.find(DEF_STOP_SECTION_CHAR);

		if((pos_start_sec != nopos && pos_stop_sec != nopos) 
				&& ((pos_start_sec + 1) < (pos_stop_sec - 1)))
		{
			if(add_section(str_line, pos_start_sec, pos_stop_sec))
				continue;
		}

		size_t pos_sep = str_line.find(DEF_SEPARATOR_CHAR);
		if(pos_sep != nopos){
			name = str_line.substr(0, pos_sep);
			val = str_line.substr(pos_sep+1, std::string::npos);

			trim(name);
			if(!name.length())
				continue;
			trim(val);

			storage.insert(str_last_sec, name, val);
		}

	}
	return true;
}

////*****************************************************
//Section* find_section(Conf *conf, const char *namesec){
//	Section *cur_sec;

//	cur_sec = conf->g_sec;

//	while(cur_sec){
//		if(cur_sec->name && (strcasecmp(cur_sec->name, namesec) == 0)){
//			return cur_sec;
//		}
//		cur_sec = cur_sec->next;
//	}
//	return NULL;
//}

//*****************************************************
// Item *find_item(const Section *section, const char *nameitem){
// 	Item *it = section->itemlist;
// 	while(it){
// 		if(nameitem && strcasecmp(it->name, nameitem) == 0){
// 			return it;
// 		}
// 		it = it->next;
// 	}
// 	return NULL;
// }

//*****************************************************
// int get_val_as_str(Conf *conf, const char *name_sec, const char *name, char **val){
// 	Section *sec = find_section(conf, name_sec);
// 	Item *it = NULL;
// 	if(sec){
// 		it = find_item(sec, name);
// 		if(it){
// 			*val = it->value;
// 			return 1;
// 		}
// 	}
// 	return 0;
// }

//*****************************************************
// int get_val_as_int(Conf *conf, const char *name_sec, const char *name, int *val){
// 	char *p_val;

// 	if(get_val_as_str(conf, name_sec, name, &p_val)){
// 		errno = 0;
// 		*val = strtol(p_val, NULL, 10);
// 		if(!errno){
// 			return 1;
// 		}
// 		fprintf(stderr, "Error (module config:conf.c) transformation CHAR->INT value [%s]:%s (%s)\n", name_sec, name, p_val);
// 	}
// 	return 0;
// }

//*****************************************************
// int get_val_as_float(Conf *conf, const char *name_sec, const char *name, float *val){
// 	char *p_val;

// 	if(get_val_as_str(conf, name_sec, name, &p_val)){
// 		errno = 0;
// 		*val = atof(p_val);
// 		if(!errno){
// 			return 1;
// 		}
// 		fprintf(stderr, "Error (module config:conf.c) transformation CHAR->FLOAT value [%s]:%s (%s)\n", name_sec, name, p_val);
// 	}
// 	return 0;
// }

//*****************************************************
void Storage::print(){
	for(const auto& e: stor){
		std::cout << "[" << e.first << "]" << std::endl;
		Item it = e.second;
		for(const auto& v : it){
			std::cout << "\t" << v.first << ": " << v.second << std::endl;
		}
		std::cout << std::endl;
	}
}

void Conf::print_conf(){
	storage.print();
}

