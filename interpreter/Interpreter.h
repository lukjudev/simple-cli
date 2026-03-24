#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>

#include "Pipeline.h"

class Interpreter {
private:
	int charLimit;
	std::string prompt;

	void executePipeline(const Pipeline& pipeline, std::ostream& out, std::ostream& err);

public:
	Interpreter() : prompt("$"), charLimit(512) {}

	void setPrompt(const std::string& p) { prompt = p; }

	void run();
	void processLine(const std::string& line);
	void processLine(const std::string& line, std::ostream& out, std::ostream& err);
};

#endif
