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
	
	// I think I'll need to switch this to a list to make the update query more efficient.
	// I'd like to be able to remove mayched entries
	std::vector<std::string> parser(const std::string& query, const char& delim) {
		std::vector<std::string> elems;
		std::stringstream ss {query};
		std::string s;

		while (std::getline(ss, s, delim)) {
			elems.push_back(s);
		}

		return elems;
	}
}

// Keyword functions
namespace kw {
	void create(const std::vector<std::string>& parsed) {
		if (util::exists(parsed[1])) {
			log::error("Create failed: file \"" + parsed[1] + "\" already exists.");
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

	void update(std::vector<std::string>& parsed) {
		if (util::exists(parsed[1])) {
			std::ifstream file(parsed[1]);
			std::ofstream tempfile(parsed[1] + ".tmp");

			if (!file || !tempfile) {
				log::error("Update failed: unable to open file \"" + parsed[1] + "\" .");
			} else {
				std::string tempstring;

				while (file >> tempstring) {
					for (unsigned long int i {parsed.size()}; i > 2; i--) {
						std::cout << parsed.size();
						std::vector<std::string> queryval {util::parser(parsed[i-1], '=')};
						std::vector<std::string> fileval {util::parser(tempstring, '=')};

						if (queryval[0] == fileval[0]) {
							tempstring = parsed[i-1];
							parsed.erase(parsed.begin() + i - 1);
						}
					}

					tempstring += '\n';
					tempfile << tempstring;
				}
			}
		} else {
			log::error("Update failed: file \"" + parsed[1] + "\" doesn't exist.");
		}
	}

	void del(const std::vector<std::string>& parsed) {
		if (util::exists(parsed[1])) {
			std::remove(parsed[1].data()); // I should error check this
		} else {
			log::error("Delete failed: file \"" + parsed[1] + "\" doesn't exist.");
		}
	}
}

void interpreter(const std::string& query) {
	std::vector<std::string> parsed {util::parser(query, ' ')};
	if (parsed.size() > 1) {
		if (parsed[0] == "CREATE") {
			kw::create(parsed);
		} else if (parsed[0] == "SELECT") {
			kw::select(parsed);
		} else if (parsed[0] == "UPDATE") {
			if (parsed.size() > 2) {
				kw::update(parsed);
			} else {
				log::error("Update failed: insufficient parameters.");
			}
		} else if (parsed[0] == "DELETE") {
			kw::del(parsed);
		} else {
			log::error("Query failed: keyword \"" + parsed[0] + "\" doesn't exist.");
		}
	} else {
		log::error("Query failed: insufficient parameters.");
	}
}

void process(const std::string& query) {
	log::query(query);
	interpreter(query);
}
