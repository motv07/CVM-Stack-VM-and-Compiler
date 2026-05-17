#include <iostream>
#include "parser.h"

Parser::Parser(std::vector<Token> toks)
{
    tokens = toks;
    current = 0;
}

bool Parser::isAtEnd() { return peek().type == TOKEN_EOF; }
Token Parser::peek() { return tokens[current]; }
Token Parser::advance()
{
    if (!isAtEnd())
        current++;
    return tokens[current - 1];
}

bool Parser::match(TokenType type)
{
    if (peek().type == type)
    {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, std::string message)
{
    if (peek().type == type)
        return advance();
    std::cout << "Parser Error Near Token: " << peek().lexeme << std::endl;
    std::cout << message << std::endl;
    exit(1);
}

// 1. PRIMARY
Expr *Parser::primary()
{
    if (match(TOKEN_NUMBER))
    {
        return new LiteralExpr(stoi(tokens[current - 1].lexeme));
    }
    if (match(TOKEN_INPUT))
    {
        consume(TOKEN_LEFT_PAREN, "Expected '(' after input");
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after input");
        return new InputExpr();
    }
    if (match(TOKEN_IDENTIFIER))
    {
        return new VariableExpr(tokens[current - 1].lexeme);
    }
    if (match(TOKEN_LEFT_PAREN))
    {
        Expr *expr = expression();
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }
    std::cout << "Parser Error: Expected expression." << std::endl;
    exit(1);
    return nullptr;
}

// 2. UNARY (Fixes the print(-1) Segfault)
Expr *Parser::unary()
{
    if (match(TOKEN_MINUS))
    {
        Expr *right = unary();
        return new BinaryExpr(new LiteralExpr(0), '-', right); // Hack: -x becomes 0 - x
    }
    return primary();
}

// 3. FACTOR (*, /)
Expr *Parser::factor()
{
    Expr *left = unary();
    while (peek().type == TOKEN_STAR || peek().type == TOKEN_SLASH)
    {
        Token op = advance();
        Expr *right = unary();
        left = new BinaryExpr(left, op.lexeme[0], right);
    }
    return left;
}

// 4. TERM (+, -)
Expr *Parser::term()
{
    Expr *left = factor();
    while (peek().type == TOKEN_PLUS || peek().type == TOKEN_MINUS)
    {
        Token op = advance();
        Expr *right = factor();
        left = new BinaryExpr(left, op.lexeme[0], right);
    }
    return left;
}

// 5. COMPARISON (<, >)
Expr *Parser::comparison()
{
    Expr *left = term();
    while (peek().type == TOKEN_GREATER || peek().type == TOKEN_LESS)
    {
        Token op = advance();
        Expr *right = term();
        left = new BinaryExpr(left, op.lexeme[0], right);
    }
    return left;
}

// 6. EQUALITY (==)
Expr *Parser::equality()
{
    Expr *left = comparison();
    while (peek().type == TOKEN_EQUAL_EQUAL)
    {
        Token op = advance();
        Expr *right = comparison();
        left = new BinaryExpr(left, '=', right);
    }
    return left;
}

// 7. EXPRESSION (Assignments)
// Update your expression() function:
Expr *Parser::expression()
{
    if (match(TOKEN_LET))
    {
        Token name = consume(TOKEN_IDENTIFIER, "Expected variable name");
        consume(TOKEN_EQUAL, "Expected '=' after variable name");
        Expr *value = expression();
        return new AssignExpr(name.lexeme, value, true); // <-- PASS TRUE
    }

    Expr *left = equality();

    if (match(TOKEN_EQUAL))
    {
        Expr *value = expression();
        VariableExpr *variable = dynamic_cast<VariableExpr *>(left);
        if (variable)
        {
            return new AssignExpr(variable->name, value, false); // <-- PASS FALSE
        }
    }
    return left;
}

// FIX: Strict Semicolon Enforcement
Stmt *Parser::statement()
{
    if (match(TOKEN_LEFT_BRACE))
    {
        std::vector<Stmt *> stmts = block();
        return new BlockStmt(stmts);
    }
    if (match(TOKEN_WHILE))
    {
        consume(TOKEN_LEFT_PAREN, "Expected '(' after while");
        Expr *condition = expression();
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after while condition");
        consume(TOKEN_LEFT_BRACE, "Expected '{' before while block");
        WhileStmt *whileStmt = new WhileStmt(condition);
        whileStmt->body = block();
        return whileStmt;
    }
    if (match(TOKEN_IF))
    {
        consume(TOKEN_LEFT_PAREN, "Expected '(' after if");
        Expr *condition = expression();
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after if condition");
        consume(TOKEN_LEFT_BRACE, "Expected '{' before if block");
        IfStmt *ifStmt = new IfStmt(condition);
        ifStmt->thenBranch = block();
        if (match(TOKEN_ELSE))
        {
            consume(TOKEN_LEFT_BRACE, "Expected '{' before else block");
            ifStmt->elseBranch = block();
        }
        return ifStmt;
    }
    if (match(TOKEN_PRINT))
    {
        consume(TOKEN_LEFT_PAREN, "Expected '(' after print");
        Expr *expr = expression();
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression");
        consume(TOKEN_SEMICOLON, "Expected ';' after print statement");
        return new PrintStmt(expr);
    }

    Expr *expr = expression();
    consume(TOKEN_SEMICOLON, "Expected ';' after expression");
    return new ExpressionStmt(expr);
}

std::vector<Stmt *> Parser::block()
{
    std::vector<Stmt *> statements;
    while (!isAtEnd() && peek().type != TOKEN_RIGHT_BRACE)
    {
        statements.push_back(statement());
    }
    consume(TOKEN_RIGHT_BRACE, "Expected '}' after block");
    return statements;
}

Program *Parser::parse()
{
    Program *program = new Program();
    while (!isAtEnd())
    {
        program->statements.push_back(statement());
    }
    return program;
}