#include "token.h"

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TOKEN_NUMBER:
        return "NUMBER";

    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";

    case TOKEN_PLUS:
        return "PLUS";

    case TOKEN_MINUS:
        return "MINUS";

    case TOKEN_STAR:
        return "STAR";

    case TOKEN_SLASH:
        return "SLASH";

    case TOKEN_EQUAL:
        return "EQUAL";

    case TOKEN_EQUAL_EQUAL:
        return "EQUAL_EQUAL";

    case TOKEN_BANG_EQUAL:
        return "BANG_EQUAL";

    case TOKEN_GREATER:
        return "GREATER";

    case TOKEN_LESS:
        return "LESS";

    case TOKEN_SEMICOLON:
        return "SEMICOLON";

    case TOKEN_LEFT_PAREN:
        return "LEFT_PAREN";

    case TOKEN_RIGHT_PAREN:
        return "RIGHT_PAREN";

    case TOKEN_LEFT_BRACE:
        return "LEFT_BRACE";

    case TOKEN_RIGHT_BRACE:
        return "RIGHT_BRACE";

    case TOKEN_PRINT:
        return "PRINT";

    case TOKEN_IF:
        return "IF";

    case TOKEN_WHILE:
        return "WHILE";

    case TOKEN_EOF:
        return "EOF";

    default:
        return "UNKNOWN";
    }
}