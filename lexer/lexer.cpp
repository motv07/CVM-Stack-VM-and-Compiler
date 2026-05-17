#include "lexer.h"
#include <iostream>
Lexer::Lexer(std::string src)
{
    source = src;

    current = 0;
}

bool Lexer::isAtEnd()
{
    return current >= source.length();
}

char Lexer::advance()
{
    return source[current++];
}

char Lexer::peek()
{
    if (isAtEnd())
    {
        return '\0';
    }

    return source[current];
}

char Lexer::peekNext()
{
    if (current + 1 >= source.length())
    {
        return '\0';
    }

    return source[current + 1];
}

bool Lexer::match(char expected)
{
    if (isAtEnd())
    {
        return false;
    }

    if (source[current] != expected)
    {
        return false;
    }

    current++;

    return true;
}

void Lexer::skipWhitespace()
{
    while (!isAtEnd())
    {
        char c = peek();

        if (c == ' ' ||
            c == '\r' ||
            c == '\n' ||
            c == '\t')
        {
            advance();
        }
        else
        {
            break;
        }
    }
}
Token Lexer::scanToken()
{
    skipWhitespace();

    if (isAtEnd())
    {
        return {TOKEN_EOF, ""};
    }

    char c = advance();

    switch (c)
    {
    case '+':
        return {TOKEN_PLUS, "+"};

    case '-':
        return {TOKEN_MINUS, "-"};

    case '*':
        return {TOKEN_STAR, "*"};

    case '/':

        if (match('/'))
        {
            while (
                peek() != '\n' &&
                !isAtEnd())
            {
                advance();
            }

            return scanToken();
        }

        return {
            TOKEN_SLASH,
            "/"};
    case '&':

        if (match('&'))
        {
            return {
                TOKEN_AND,
                "&&"};
        }

        break;

    case '|':

        if (match('|'))
        {
            return {
                TOKEN_OR,
                "||"};
        }

        break;
    case '=':

        if (match('='))
        {
            return {
                TOKEN_EQUAL_EQUAL,
                "=="};
        }

        return {
            TOKEN_EQUAL,
            "="};

    case '!':

        if (match('='))
        {
            return {
                TOKEN_BANG_EQUAL,
                "!="};
        }

        break;

    case '>':
        return {TOKEN_GREATER, ">"};

    case '<':
        return {TOKEN_LESS, "<"};

    case ';':
        return {TOKEN_SEMICOLON, ";"};

    case '(':
        return {TOKEN_LEFT_PAREN, "("};

    case ')':
        return {TOKEN_RIGHT_PAREN, ")"};

    case '{':
        return {TOKEN_LEFT_BRACE, "{"};

    case '}':
        return {TOKEN_RIGHT_BRACE, "}"};
    }

    if (isdigit(c))
    {
        std::string number;

        number += c;

        while (isdigit(peek()))
        {
            number += advance();
        }

        return {TOKEN_NUMBER, number};
    }

    if (isalpha(c))
    {
        std::string identifier;

        identifier += c;

        while (isalnum(peek()))
        {
            identifier += advance();
        }

        if (identifier == "print")
        {
            return {TOKEN_PRINT, identifier};
        }

        if (identifier == "if")
        {
            return {TOKEN_IF, identifier};
        }

        if (identifier == "while")
        {
            return {TOKEN_WHILE, identifier};
        }
        if (identifier == "else")
        {
            return {TOKEN_ELSE, identifier};
        }
        if (identifier == "input")
        {
            return {TOKEN_INPUT, identifier};
        }
        if (identifier == "let")
        {
            return {TOKEN_LET, identifier};
        }
        return {
            TOKEN_IDENTIFIER,
            identifier};
    }

    std::cout
        << "Lexer Error: Unexpected character: "
        << c
        << std::endl;

    exit(1);
}

std::vector<Token>
Lexer::scanTokens()
{
    std::vector<Token> tokens;

    while (true)
    {
        Token token =
            scanToken();

        tokens.push_back(token);

        if (token.type ==
            TOKEN_EOF)
        {
            break;
        }
    }

    return tokens;
}
