#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "Token.h"

class Lexer {
public:
	Lexer() {}
	
	std::vector<Token> tokenize(const std::string& line);
};

#endif