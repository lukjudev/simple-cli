#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "CommandSegment.h"
#include "Pipeline.h"
#include "Token.h"

class Parser {
private:
	std::vector<CommandSegment> splitLine(const std::vector<Token>& tokens);
	void validateLine(const std::vector<CommandSegment>& segments);

public:
	Parser() {}

	Pipeline parseLine(const std::vector<Token>& tokens);
};

#endif
