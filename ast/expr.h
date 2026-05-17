#include <string>
#pragma once

class Expr
{

public:
    virtual ~Expr() {}
};

class LiteralExpr : public Expr
{

public:
    int value;

    LiteralExpr(int val)
    {
        value = val;
    }
};

class VariableExpr : public Expr
{

public:
    std::string name;

    VariableExpr(std::string n)
    {
        name = n;
    }
};
class AssignExpr : public Expr
{
public:
    std::string name;
    Expr *value;
    bool isLet; // <-- NEW: Tracks if this is a declaration

    AssignExpr(std::string n, Expr *val, bool letFlag = false)
    {
        name = n;
        value = val;
        isLet = letFlag; // <-- Set the flag
    }
};

class BinaryExpr : public Expr
{

public:
    Expr *left;

    Expr *right;

    char op;

    BinaryExpr(Expr *l, char o, Expr *r)
    {
        left = l;
        op = o;
        right = r;
    }
};

class InputExpr : public Expr
{
};