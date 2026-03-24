#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <vector>

#include "InterpreterErrors.h"
#include "Lexer.h"

namespace {

	bool isWhitespace(char c) {
		return c == ' ' || c == '\t' || c == '\r';
	}


	bool isAllowedNormalChar(unsigned char c) {
		return std::isalnum(c) != 0 || c == '.' || c == '_' || c == '-' || c == '/' || c == '\\' || c == ':';
	}

	// pretpostavlja sortiran niz
	void removeDuplicatePositions(std::vector<std::size_t>& positions) {
		std::size_t nextIndex = 0;
		for (std::size_t i = 0; i < positions.size(); i++) {
			if (nextIndex == 0 || positions[i] != positions[nextIndex - 1]) {
				positions[nextIndex++] = positions[i];
			}
		}
		positions.resize(nextIndex);
	}

	std::string formatLexicalError(const std::string& line, std::vector<std::size_t> positions) {
		std::sort(positions.begin(), positions.end());
		removeDuplicatePositions(positions);

		std::string msg = "Leksicka greska: neocekivani znakovi\n";
		msg += line + '\n';
		std::string underline(line.size(), ' ');
		for (std::size_t pos : positions) {
			if (pos < underline.size()) {
				underline[pos] = '^';
			}
		}
		msg += underline;
		return msg;
	}

}

std::vector<Token> Lexer::tokenize(const std::string& line) {
	std::vector<Token> tokens;
	std::vector<std::size_t> badPositions;
	std::size_t i = 0;
	const std::size_t n = line.size();

	while (true) {
		while (i < n && isWhitespace(line[i])) {
			i++;
		}
		if (i >= n) {
			break;
		}

		if (line[i] == '"') {
			const std::size_t openingQuotePos = i;
			i++;
			std::string curr;
			while (i < n && line[i] != '"') {
				curr.push_back(line[i]);
				i++;
			}
			if (i >= n) {
				throw LexicalError("Leksicka greska: navodnici na poziciji " + std::to_string(openingQuotePos) + " nemaju svog para");
			}
			i++;
			tokens.push_back(Token{ Quoted, curr });
			continue;
		}

		if (line[i] == '|') {
			i++;
			tokens.push_back(Token{ Pipe, "|" });
			continue;
		}

		if (line[i] == '<') {
			i++;
			tokens.push_back(Token{ RedirectIn, "<" });
			continue;
		}

		if (line[i] == '>') {
			i++;
			if (i < n && line[i] == '>') {
				i++;
				tokens.push_back(Token{ RedirectOutAppend, ">>" });
			}
			else {
				tokens.push_back(Token{ RedirectOut, ">" });
			}
			continue;
		}

		const std::size_t normalStart = i;
		std::string normal;
		while (i < n) {
			const char c = line[i];
			if (isWhitespace(c) || c == '"' || c == '|' || c == '<' || c == '>') {
				break;
			}
			normal.push_back(c);
			i++;
		}
		if (!normal.empty()) {
			for (std::size_t j = 0; j < normal.size(); ++j) {
				char c = normal[j];
				if (!isAllowedNormalChar(c)) {
					badPositions.push_back(normalStart + j);
				}
			}
			tokens.push_back(Token{ Normal, normal });
		}
	}

	if (!badPositions.empty()) {
		throw LexicalError(formatLexicalError(line, badPositions));
	}

	return tokens;
}
