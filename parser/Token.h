#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType { Normal, Quoted, Pipe, RedirectIn, RedirectOut, RedirectOutAppend };

struct Token {
	TokenType type;
	std::string text;
};

#endif