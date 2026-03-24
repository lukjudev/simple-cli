#include <fstream>
#include <string>
#include <vector>

#include "Command.h"
#include "InterpreterErrors.h"
#include "TruncateCommand.h"

TruncateCommand::TruncateCommand(CommandSegment segment) : Command(segment) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande truncate");
	}

	if (n != 2) {
		throw SyntaxError("Sintaksna greska: neispravan format komande truncate");
	}

	if (tokens[0].type != Normal || tokens[0].text != "truncate") {
		throw SyntaxError("Sintaksna greska: neispravan format komande truncate");
	}

	if (tokens[1].type != Normal) {
		throw SyntaxError("Sintaksna greska: neispravan format komande truncate");
	}
}

void TruncateCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::string& path = segment.tokens[1].text;
	// skrati fajl na 0 bajtova
	std::ofstream file(path, std::ios::binary | std::ios::trunc);
	if (!file) {
		throw CommandExecutionError("Greska: nije moguce isprazniti datoteku '" + path + "'");
	}
}
