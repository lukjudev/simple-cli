#ifndef COMMAND_SEGMENT_H
#define COMMAND_SEGMENT_H

#include <vector>

#include "Token.h"

struct CommandSegment {
	std::vector<Token> tokens;
};

#endif