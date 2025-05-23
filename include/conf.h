#ifndef __CONF_H__
#define __CONF_H__

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <list>
 

class Storage{
	public:
		Storage(){};
		~Storage(){};

		void insert(const std::string&, const std::string &name, const std::string &val);
		const std::string& getval(const std::string&, const std::string&);
		void print();
	private:
		typedef std::unordered_map<std::string, std::string> Items;
		typedef std::unordered_map<std::string, Items> Sections;
		Sections stor;
};

static std::string ret_null_val{"{null}"};

class Conf{
	public:
		Conf();
		~Conf(){};

		bool read_conf(const char *namefile);

		const char*				get_val_as_cstr(const char *name_sec, const char *name);
		bool					get_val_as_str(const std::string& name_sec, const std::string& name, const std::string& val);

		bool					get_val_as_int(const std::string& name_sec, const std::string& name, int *val);
		bool					get_val_as_float(const std::string& name_sec, const std::string& name, float *val);
		bool					get_val_as_double(const std::string& name_sec, const std::string& name, double *val);

		void					print_conf();

	private:
		void trim(std::string &str);
		bool add_section(const std::string &str_line, size_t pos_start_sec, size_t pos_stop_sec);
		bool add_file_list(const char *namef);

		typedef struct iof{ ino_t inode; std::string name; } iof;
		Storage					storage;
		std::string				str_last_sec;
		std::list<ino_t>		files;
};


#endif
