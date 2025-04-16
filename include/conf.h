#ifndef __CONF_H__
#define __CONF_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
 
typedef std::unordered_map<std::string, std::string> Item;

class Storage{
	public:
		Storage();
		~Storage(){};

		void insert(const std::string&, const std::string &name, const std::string &val);
		const Item* get(const std::string&);
		void print();
	private:
		std::unordered_map<std::string, Item> stor;
};

class Conf{
	public:
		Conf();
		~Conf(){};

		bool read_conf(char *namefile);

		int get_val_as_str( const char *name_sec, const char *name, char **val);
		int get_val_as_int(const char *name_sec, const char *name, int *val);
		int get_val_as_float(const char *name_sec, const char *name, float *val);
		int get_val_as_double(const char *name_sec, const char *name, double *val);

		void print_conf();

	private:
		void trim(std::string &str);
		bool add_section(const std::string &str_line, size_t pos_start_sec, size_t pos_stop_sec);
		Storage storage;
		std::string str_last_sec;
};


#endif
