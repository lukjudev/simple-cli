#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Command.h"
#include "HeadCommand.h"
#include "InterpreterErrors.h"

HeadCommand::HeadCommand(CommandSegment segment) : Command(segment) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	if (n < 2 || n > 3) {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	if (tokens[0].type != Normal || tokens[0].text != "head") {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	if (tokens[1].type != Normal) {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	const std::string& opt = tokens[1].text;
	if (opt.size() < 3 || opt.size() > 7 || opt.substr(0, 2) != "-n") {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	for (std::size_t i = 2; i < opt.size(); i++) {
		if (!std::isdigit(static_cast<unsigned char>(opt[i]))) {
			throw SyntaxError("Sintaksna greska: neispravan format komande head");
		}
	}

	if (n == 3) {
		if (tokens[2].type != Normal && tokens[2].type != Quoted) {
			throw SyntaxError("Sintaksna greska: neispravan format komande head");
		}
	}
}

void HeadCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::string& opt = tokens[1].text;

	if (opt.size() < 3) {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	const std::string numStr = opt.substr(2);
	if (numStr.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	unsigned long n = 0;
	try {
		n = std::stoul(numStr);
	}
	catch (...) {
		throw SyntaxError("Sintaksna greska: neispravan format komande head");
	}

	std::istream* src = &in;
	std::ifstream file;
	std::istringstream quotedIn;
	if (tokens.size() == 3) {
		const Token& arg = tokens[2];
		if (arg.type == Quoted) {
			quotedIn.str(arg.text);
			quotedIn.clear();
			src = &quotedIn;
		} else {
			const std::string& path = arg.text;
			file.open(path);
			if (!file) {
				throw CommandExecutionError("Greska: nije moguce otvoriti datoteku '" + path + "'");
			}
			src = &file;
		}
	}

	std::string content;
	std::string line;
	std::size_t i = 0;
	while (std::getline(*src, line)) {
		if (i < n) {
			content += line + "\n";
			i++;
		}
	}
	if (!content.empty() && content[content.size() - 1] == '\n') {
		content = content.substr(0, content.size() - 1);
	}
	out << content;
	if (&out == &std::cout) {
		out << '\n';
	}
}
