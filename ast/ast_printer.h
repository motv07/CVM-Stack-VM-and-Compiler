#pragma once
#include "stmt.h"
#include "expr.h"
#include <string>
#include <vector>

class ASTPrinter {
public:
    std::string print(Program* program);
private:
    std::string printExpr(Expr* expr);
    std::string printStmt(Stmt* stmt);
    std::string printBlock(std::vector<Stmt*>& stmts);
};
