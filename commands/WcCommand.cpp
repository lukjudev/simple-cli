#include <cctype>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "Command.h"
#include "InterpreterErrors.h"
#include "WcCommand.h"

WcCommand::WcCommand(CommandSegment segment) : Command(segment) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::size_t n = tokens.size();

	if (tokens.empty()) {
		throw SyntaxError("Sintaksna greska: neispravan format komande wc");
	}
	if (tokens[0].type != Normal || tokens[0].text != "wc") {
		throw SyntaxError("Sintaksna greska: neispravan format komande wc");
	}
	if (n < 2) {
		throw SyntaxError("Sintaksna greska: neispravan format komande wc");
	}
	if (tokens[1].type != Normal || (tokens[1].text != "-w" && tokens[1].text != "-c")) {
		throw SyntaxError("Sintaksna greska: neispravan format komande wc");
	}
	if (n == 3 && tokens[2].type != Normal && tokens[2].type != Quoted) {
		throw SyntaxError("Sintaksna greska: neispravan format komande wc");
	}
	if (n > 3) {
		throw SyntaxError("Sintaksna greska: neispravan format komande wc");
	}
}

void WcCommand::execute(std::istream& in, std::ostream& out, std::ostream& err) {
	const std::vector<Token>& tokens = segment.tokens;
	const std::string& opt = tokens[1].text;

	std::istream* src = &in;
	std::ifstream file;
	std::istringstream quotedIn;

	// ako je argument pod navodnicima, uzimamo ga kao tekstualnu vrednost, u suprotnom otvaramo fajl
	if (tokens.size() == 3) {
		const Token& arg = tokens[2];
		if (arg.type == Quoted) {
			quotedIn.str(arg.text);
			quotedIn.clear();
			src = &quotedIn;
		} else {
			file.open(arg.text);
			if (!file) {
				throw CommandExecutionError("Greska: nije moguce otvoriti datoteku '" + arg.text + "'");
			}
			src = &file;
		}
	}

	if (opt == "-w") {
		std::string content;
		std::string line;
		while (std::getline(*src, line)) {
			content += line + '\n';
		}
		if (!content.empty() && content[content.size() - 1] == '\n') {
			content = content.substr(0, content.size() - 1);
		}

		std::size_t count = 0;
		bool inWord = false;
		const std::size_t n = content.size();
		for (std::size_t i = 0; i < n; i++) {
			char c = content[i];
			if (std::isspace(c) != 0) {
				inWord = false;
			} else {
				if (!inWord) {
					count++;
					inWord = true;
				}
			}
		}
		out << count << '\n';
		return;
	}

	if (opt == "-c") {
		const std::string content((std::istreambuf_iterator<char>(*src)), std::istreambuf_iterator<char>());
		out << content.size() << '\n';
	}
}
