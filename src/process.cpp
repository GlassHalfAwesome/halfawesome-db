#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <syslog.h>

bool exists(const std::string& filename) {
	struct stat buffer;   

	return stat(filename.data(), &buffer) == 0; 
}

std::vector<std::string> parser(const std::string& query) {
	std::vector<std::string> elems;
	std::stringstream ss {query};
	std::string s;

	while (std::getline(ss, s, ' ') {
	elems.push_back(s);
	}

	return elems;
}

void create(const std::string& filename) {
	if (exists(filename)) {
		syslog(LOG_ERR, "Create failed: file already exists.");
	} else {
		std::ofstream file;

		file.open(filename); // I should error check this
		file << "Creating.\n";
		file.close();
	}

}

void update(const std::string& filename) {
	if (exists(filename)) {
		std::ofstream file;

		file.open(filename, std::ios_base::app); // I should error check this
		file << "Appending.\n";
		file.close();
	} else {
		syslog(LOG_ERR, "Update failed: file doesn't exist.");
	}
}

void del(const std::string& filename) {
	if (exists(filename)) {
		std::remove(filename.data()); // I should error check this
	} else {
		syslog(LOG_ERR, "Delete failed: file doesn't exist.");
	}
}

void process() {
	std::time_t timestamp {std::time(nullptr)};
	std::stringstream ss;

	ss << timestamp << ".txt";

	std::string filename = ss.str();
	create(filename);
	update(filename);
	sleep(2);
	del(filename);
}
