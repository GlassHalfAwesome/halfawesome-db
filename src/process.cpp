#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <syslog.h>

void error(const std::string& error) {
	std::cout << error << '\n';	
}

bool exists(const std::string& filename) {
	struct stat buffer;   

	return stat(filename.data(), &buffer) == 0; 
}

std::vector<std::string> parser(const std::string& query) {
	std::vector<std::string> elems;
	std::stringstream ss {query};
	std::string s;

	while (std::getline(ss, s, ' ')) {
	elems.push_back(s);
	}

	return elems;
}

void create(const std::vector<std::string>& parsed) {
	if (exists(parsed[1])) {
		error("Create failed: file already exists.");
	} else {
		std::ofstream file;

		file.open(parsed[1]); // I should error check this
		file << "Creating.\n";
		file.close();
	}

}

void select(const std::vector<std::string>& parsed) {
	// Do work
}

void update(const std::vector<std::string>& parsed) {
	if (exists(parsed[1])) {
		std::ofstream file;

		file.open(parsed[1], std::ios_base::app); // I should error check this
		file << "Appending.\n";
		file.close();
	} else {
		error("Update failed: file doesn't exist.");
	}
}

void del(const std::vector<std::string>& parsed) {
	if (exists(parsed[1])) {
		std::remove(parsed[1].data()); // I should error check this
	} else {
		error("Delete failed: file doesn't exist.");
	}
}

void interpreter(const std::string& query) {
	std::vector<std::string> parsed {parser(query)};

	if (parsed[0] == "CREATE") {
		create(parsed);
	} else if (parsed[0] == "SELECT") {
		select(parsed);
	} else if (parsed[0] == "UPDATE") {
		update(parsed);
	} else if (parsed[0] == "DELETE") {
		del(parsed);
	} else {
		error("Your query sucks.");
	}
}

void process(const std::string& query) {
	std::cout << query << '\n';
	interpreter(query);
}
