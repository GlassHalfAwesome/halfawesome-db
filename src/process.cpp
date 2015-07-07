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
	void create(const std::vector<std::string>& parsedQuery) {
		if (util::exists(parsedQuery[1])) {
			log::error("Create failed: file \"" + parsedQuery[1] + "\" already exists.");
		} else {
			std::ofstream file;

			file.open(parsedQuery[1]); // I should error check this
			file << "Creating.\n";
			file.close();
		}
	}

	void select(const std::vector<std::string>& parsedQuery) {
		// Do work
	}

	void update(std::vector<std::string>& parsedQuery) {
		if (util::exists(parsedQuery[1])) {
			std::string oldfn{parsedQuery[1]};
			std::string newfn{parsedQuery[1]  + ".tmp"};
			std::ifstream file(oldfn);
			std::ofstream tempfile(newfn);

			if (!file || !tempfile) {
				if (!file) {
					log::error("Update failed: unable to open file \"" + parsedQuery[1] + "\" .");
				} else {
					log::error("Update failed: unable to create file \"" + parsedQuery[1] + ".tmp\" .");
				}
			} else {
				std::string tempstring;
				
				int t {0}; // Counter for tab formatting
				while (file >> tempstring) {
					// Parses the update parameters
					for (unsigned long int j {parsedQuery.size()}; j > 2; j--) {
						std::vector<std::string> queryval {util::parser(parsedQuery[j - 1], '=')};
						std::vector<std::string> fileval {util::parser(tempstring, '=')};
						// Matches and replaces file data with query data
						if (queryval[0] == fileval[0]) {
							tempstring = parsedQuery[j - 1];
							parsedQuery.erase(parsedQuery.begin() + j - 1);
						}
					}

					// Formats the output with tabs and commas
					if (tempstring.find("}") != std::string::npos) { t--; }
					for (int k = 0; k < t; k++) { tempstring = '\t' + tempstring; }
					if (tempstring.find("{") != std::string::npos) { t++; }

					tempstring += '\n';
					tempfile << tempstring;
				}

				std::remove(oldfn.data()); // I should error check this
				std::rename(newfn.data(), oldfn.data()); // I should error check this
			}
		} else {
			log::error("Update failed: file \"" + parsedQuery[1] + "\" doesn't exist.");
		}
	}

	void del(const std::vector<std::string>& parsedQuery) {
		if (util::exists(parsedQuery[1])) {
			std::remove(parsedQuery[1].data()); // I should error check this
		} else {
			log::error("Delete failed: file \"" + parsedQuery[1] + "\" doesn't exist.");
		}
	}
}

// Using if statements instead of switch to avoid having to map an enum to strings.
// Should probably change it someday to help pretty up the code.
void interpreter(const std::string& query) {
	std::vector<std::string> parsedQuery {util::parser(query, ' ')};
	unsigned long int size {parsedQuery.size()};
	if (parsedQuery.size() > 1) {
		if (parsedQuery[0] == "CREATE" && size == 2) {
			kw::create(parsedQuery);
		} else if (parsedQuery[0] == "SELECT" && size == 2) {
			kw::select(parsedQuery);
		} else if (parsedQuery[0] == "UPDATE" && size > 2) {
			if (parsedQuery.size() > 2) {
				kw::update(parsedQuery);
			} else {
				log::error("Update failed: insufficient parameters.");
			}
		} else if (parsedQuery[0] == "DELETE" && size == 2) {
			kw::del(parsedQuery);
		} else {
			switch (size) {
				case 0:
					log::error("Query failed: empty query.");
					break;
				case 1:
					log::error("Query failed: no filename.");
					break;
				case 2:
					log::error("Query failed: keyword \"" + parsedQuery[0] + "\" doesn't exist.");
					break;
				default:
					log::error("Query failed: too many parameters.");
					break;
			}
		}
	} else {
		log::error("Query failed: insufficient parameters.");
	}
}

void process(const std::string& query) {
	log::query(query);
	interpreter(query);
}
