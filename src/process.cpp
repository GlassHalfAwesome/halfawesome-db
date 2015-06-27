#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <syslog.h>

bool exists(const std::string& filename) {
	struct stat buffer;   

	return stat(filename.c_str(), &buffer) == 0; 
}

void create(const std::string& filename) {
	if (exists(filename)) {
		syslog(LOG_ERR, "Create failed: file already exists.");
	} else {
		std::ofstream testfile;

		testfile.open(filename);
		testfile << "Writing to a file.\n";
		testfile.close();
	}

}

void process() {
	std::time_t timestamp {std::time(nullptr)};
	std::stringstream ss;

	ss << timestamp << ".txt";

	std::string filename = ss.str();
	create(filename);
}
