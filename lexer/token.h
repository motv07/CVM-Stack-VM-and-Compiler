#pragma once

#include <string>

enum TokenType
{

    TOKEN_NUMBER,

    TOKEN_IDENTIFIER,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,

    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG_EQUAL,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_GREATER,
    TOKEN_LESS,

    TOKEN_SEMICOLON,

    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,

    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,

    TOKEN_PRINT,
    TOKEN_INPUT,
    TOKEN_IF,
    TOKEN_WHILE,
    TOKEN_ELSE,
    TOKEN_LET,
    TOKEN_EOF
};

struct Token
{

    TokenType type;

    std::string lexeme;
};

std::string tokenTypeToString(TokenType type);