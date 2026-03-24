#include <fstream>
#include <string>
#include <vector>

#include "BatchCommand.h"
#include "Command.h"
#include "Interpreter.h"
#include "InterpreterErrors.h"

BatchCommand::BatchCommand(CommandSegment segment, Interpreter& interpreter) : Command(segment), interpreter(interpreter) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande batch");
	}

	if (n != 2) {
		throw SyntaxError("Sintaksna greska: neispravan format komande batch");
	}

	if (tokens[0].type != Normal || tokens[0].text != "batch") {
		throw SyntaxError("Sintaksna greska: neispravan format komande batch");
	}

	if (tokens[1].type != Normal) {
		throw SyntaxError("Sintaksna greska: neispravan format komande batch");
	}
}

void BatchCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::string& path = segment.tokens[1].text;
	std::ifstream file(path);
	if (!file) {
		throw CommandExecutionError("Greska: nije moguce otvoriti datoteku '" + path + "'");
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}
		// svaka linija iz fajla kao da je kucana na konzoli
		interpreter.processLine(line, out, err);
	}
}
