#include <string>
#include <ifstream>
#include "file.h"

using namespace std;

File_Conf::File_Conf(const string* _name)
:name(_name)
{
	if(stream.is_open())
		stream.close();
	stream.open(name);
}

File_Conf::~File_Conf(){
	if(stream.is_open())
		stream.close();
}

string * File_Conf::readline(){
	last_read_line << stream;
	return &last_read_line;
}

