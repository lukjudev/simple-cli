#ifndef PIPELINE_H
#define PIPELINE_H

#include <string>
#include <vector>

#include "CommandSegment.h"

struct Pipeline {
	std::vector<CommandSegment> segments;
	std::string inStream;
	std::string outStream;
	bool appendOutStream;
};

#endif