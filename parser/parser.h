#pragma once
#include <vector>
#include "../lexer/token.h"
#include "../ast/expr.h"
#include "../ast/stmt.h"

class Parser
{
private:
    std::vector<Token> tokens;
    int current;

public:
    Parser(std::vector<Token> toks);
    Program *parse();
    Stmt *statement();
    std::vector<Stmt *> block();

    // Proper recursive descent layers for precedence
    Expr *expression();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *primary();

    bool match(TokenType type);
    Token consume(TokenType type, std::string message);
    Token advance();
    Token peek();
    bool isAtEnd();
};