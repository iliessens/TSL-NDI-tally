
#include "SourceMapper.h"

#define MAPPING_FILE "tallymap.txt"

SourceMapper::SourceMapper(NDILib_loader* loader)
{
	ifstream file(MAPPING_FILE);
	string line;

	if (!file) {
		printf("File %s does not exist!\n", MAPPING_FILE);
		throw runtime_error("Tally map not found");
	}

	// for each line (newlines are gobbled)
	while (getline(file, line)) {
		vector<string> result;
		stringstream ss(line);
		string item;

		// for each part of line
		while (getline(ss, item, ':')) {
			result.push_back(item);
		}

		try {
			// Check if parts are present
			if (result.size() < 2) {
				throw runtime_error("Invalid config format");
			}

			int id = stoi(result.at(0)); // get ID
			string name = result.at(1);
			printf("Source mapped %d => %s\n", id, name.c_str());
			this->sources[id] = new NDITally(loader, name); // Create connection and add to map
		}
		catch (std::exception& e) {
			printf("Skipped line %s due to exception\n", line.c_str());
			printf("Message %s \n", e.what());
		}
	}
}

SourceMapper::~SourceMapper() {
	map<int, NDITally*>::iterator it;
	for (it = sources.begin(); it != sources.end(); ++it) {
		delete it->second;
	}
}

NDITally* SourceMapper::id_to_source(int id)
{
	if (sources.count(id) == 0) {
		return NULL;
	}

	return sources[id];
}
