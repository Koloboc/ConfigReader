#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "conf.h"
#include "defines.h"

const std::string& Storage::getval(const std::string& name_sec, const std::string& name){
	if(auto sec_it = stor.find(name_sec) ; sec_it != stor.end()){
		if(auto param_it = (*sec_it).second.find(name) ; param_it != (*sec_it).second.end()){
			return (*param_it).second;
		}
	}
	return ret_null_val;
}

void Storage::insert(const std::string& name_sec, const std::string &name, const std::string &val){
	Sections::iterator sec_iter;
	sec_iter = stor.find(name_sec);
	if(sec_iter == stor.end()){ // not find
		Items item_it{{name, val}};
		stor[name_sec] = item_it;
	}else{		// find
		(*sec_iter).second.insert_or_assign(name, val);
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
bool Conf::read_conf(const char *namef){

	std::string str_save_sec;
	std::string str_line;
	size_t nopos = std::string::npos;

	if(!add_file_list(namef))
		return false;

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

			if(name == "Include"){
				str_save_sec = str_last_sec;
				if( ! read_conf(val.c_str()) )
					return false;
				str_last_sec = str_save_sec;
				continue;
			}
			storage.insert(str_last_sec, name, val);
		}
	}
	return true;
}

//*****************************************************
bool Conf::add_file_list(const char *namef){
	struct stat st;
	std::string name(namef);
	if(stat(namef, &st) != 0)
		return false;

	if(std::find(files.begin(), files.end(), st.st_ino) != files.end()){
		return false;
	}
	files.push_back(st.st_ino);
	return true;
}

//*****************************************************
const char* Conf::get_val_as_cstr(const char *section, const char *name){
	if(!section || !strlen(section))
		return storage.getval(DEF_SECTION_NAME, name).c_str();
	return storage.getval(section, name).c_str();
}

//*****************************************************
bool
Conf::get_val_as_str(const std::string &section, const std::string& name, const std::string& val){
	std::string name_sec = section;
	if(name_sec.empty())
		name_sec = DEF_SECTION_NAME;
	return storage.getval(name_sec, name).c_str() == ret_null_val ? false : true;
}

//*****************************************************
bool Conf::get_val_as_int(const std::string& section, const std::string& name, int *val){
	std::string name_sec = section;
	int ret;

	if(name_sec.empty())
		name_sec = DEF_SECTION_NAME;

	std::string ansv = storage.getval(name_sec, name);
	try{
		ret = std::stoi(ansv);
	}catch(...){
		return false;
	}
	*val = ret;
	return true;
}

//*****************************************************
bool Conf::get_val_as_float(const std::string& section, const std::string& name, float *val){
	std::string name_sec = section;
	float ret;

	if(name_sec.empty())
		name_sec = DEF_SECTION_NAME;

	std::string ansv = storage.getval(name_sec, name);
	try{
		ret = std::stof(ansv);
	}catch(...){
		return false;
	}
	*val = ret;
	return true;
}

//*****************************************************
bool Conf::get_val_as_double(const std::string& section, const std::string& name, double *val){
	std::string name_sec = section;
	double ret;

	if(name_sec.empty())
		name_sec = DEF_SECTION_NAME;

	std::string ansv = storage.getval(name_sec, name);
	try{
		ret = std::stod(ansv);
	}catch(...){
		return false;
	}
	*val = ret;
	return true;
}

//*****************************************************
void Storage::print(){
	for(const auto& e: stor){
		std::cout << "[" << e.first << "]" << std::endl;
		Items it = e.second;
		for(const auto& v : it){
			std::cout << "\t" << v.first << ": " << v.second << std::endl;
		}
		std::cout << std::endl;
	}
}

void Conf::print_conf(){
	storage.print();
}

