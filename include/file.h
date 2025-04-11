#ifndef __FILE_H__
#define __FILE_H__

#include <string>
#include <fstream>

using namespace std;

class File_Conf{
	public:
		File_Conf(const string* name);
		~File_Conf();

		string * readline();
	private:
		ifstream *stream;
		string name;
		string last_read_line;
}

#endif
