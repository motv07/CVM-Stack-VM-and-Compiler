#pragma once

#include <vector>

#include "expr.h"

class Stmt
{
public:
    virtual ~Stmt() {}
};
class ExpressionStmt : public Stmt
{
public:
    Expr *expression;

    ExpressionStmt(Expr *expr)
    {
        expression = expr;
    }
};

class PrintStmt : public Stmt
{
public:
    Expr *expression;

    PrintStmt(Expr *expr)
    {
        expression = expr;
    }
};

class Program
{
public:
    std::vector<Stmt *> statements;
};

class IfStmt : public Stmt
{
public:
    Expr *condition;

    std::vector<Stmt *> thenBranch;

    std::vector<Stmt *> elseBranch;

    IfStmt(Expr *cond)
    {
        condition = cond;
    }
};

class WhileStmt : public Stmt
{
public:
    Expr *condition;

    std::vector<Stmt *> body;

    WhileStmt(Expr *cond)
    {
        condition = cond;
    }
};
class BlockStmt : public Stmt
{
public:
    std::vector<Stmt *> body;
    BlockStmt(std::vector<Stmt *> stmts)
    {
        body = stmts;
    }
};