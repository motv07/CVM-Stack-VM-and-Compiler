#pragma once

#include <vector>

#include "token.h"

class Lexer
{

private:
    std::string source;

    int current;

public:
    Lexer(std::string src);

    std::vector<Token> scanTokens();

    Token scanToken();

    char advance();

    char peek();
    char peekNext();

    bool match(char expected);

    bool isAtEnd();

    void skipWhitespace();
};