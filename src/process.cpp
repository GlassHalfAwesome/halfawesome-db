#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>

// Log functions
namespace log {
	void error(const std::string& error) {
		std::ofstream file;

		file.open("logs/errors", std::ios_base::app); // I should error check this
		file << error << '\n';
		file.close();
	}

	void query(const std::string& query) {
		std::ofstream file;

		file.open("logs/queries", std::ios_base::app); // I should error check this
		file << query << '\n';
		file.close();
	}
}

// Utility functions
namespace util {
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
}

// Keyword functions
namespace kw {
	void create(const std::vector<std::string>& parsed) {
		if (util::exists(parsed[1])) {
			log::error("Create failed: file already exists.");
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
		if (util::exists(parsed[1])) {
			std::ofstream file;

			file.open(parsed[1], std::ios_base::app); // I should error check this
			for (unsigned int i = 2; i < parsed.size(); i++) {
				file << parsed[i] << '\n';
			}
			file.close();
		} else {
			log::error("Update failed: file doesn't exist.");
		}
	}

	void del(const std::vector<std::string>& parsed) {
		if (util::exists(parsed[1])) {
			std::remove(parsed[1].data()); // I should error check this
		} else {
			log::error("Delete failed: file doesn't exist.");
		}
	}
}

void interpreter(const std::string& query) {
	std::vector<std::string> parsed {util::parser(query)};
	if (parsed.size() > 1) {
		if (parsed[0] == "CREATE") {
			kw::create(parsed);
		} else if (parsed[0] == "SELECT") {
			kw::select(parsed);
		} else if (parsed[0] == "UPDATE") {
			if (parsed.size() > 2) {
				kw::update(parsed);
			}
		} else if (parsed[0] == "DELETE") {
			kw::del(parsed);
		} else {
			log::error("Query failed: keyword doesn't exist.");
		}
	} else {
		log::error("Query failed: insufficient parameters.");
	}
}

void process(const std::string& query) {
	log::query(query);
	interpreter(query);
}
