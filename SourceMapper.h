#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string> 

#include "NDITally.h"
#include "NDIlib_loader.h"

using namespace std;

class SourceMapper {
public: 
	SourceMapper(NDILib_loader* loader);
	~SourceMapper();
	NDITally* id_to_source(int id);

private:
	map<int,NDITally*> sources;
};