#include "ast_printer.h"
#include <string>

std::string ASTPrinter::printExpr(Expr* expr) {
    if (!expr) return "null";

    if (auto e = dynamic_cast<LiteralExpr*>(expr)) {
        return std::to_string(e->value);
    }
    if (auto e = dynamic_cast<VariableExpr*>(expr)) {
        return e->name;
    }
    if (auto e = dynamic_cast<AssignExpr*>(expr)) {
        return "(assign " + e->name + " " + printExpr(e->value) + ")";
    }
    if (auto e = dynamic_cast<BinaryExpr*>(expr)) {
        return "(" + std::string(1, e->op) + " "
               + printExpr(e->left) + " "
               + printExpr(e->right) + ")";
    }
    if (dynamic_cast<InputExpr*>(expr)) {
        return "(input)";
    }
    return "unknown-expr";
}

std::string ASTPrinter::printBlock(std::vector<Stmt*>& stmts) {
    std::string result;
    for (auto s : stmts) {
        result += printStmt(s) + " ";
    }
    return result;
}

std::string ASTPrinter::printStmt(Stmt* stmt) {
    if (!stmt) return "null";

    if (auto s = dynamic_cast<PrintStmt*>(stmt)) {
        return "(print " + printExpr(s->expression) + ")";
    }
    if (auto s = dynamic_cast<ExpressionStmt*>(stmt)) {
        return "(expr " + printExpr(s->expression) + ")";
    }
    if (auto s = dynamic_cast<IfStmt*>(stmt)) {
        std::string result = "(if " + printExpr(s->condition)
                           + " (then " + printBlock(s->thenBranch) + ")";
        if (!s->elseBranch.empty()) {
            result += " (else " + printBlock(s->elseBranch) + ")";
        }
        result += ")";
        return result;
    }
    if (auto s = dynamic_cast<WhileStmt*>(stmt)) {
        return "(while " + printExpr(s->condition)
               + " " + printBlock(s->body) + ")";
    }
    return "unknown-stmt";
}

std::string ASTPrinter::print(Program* program) {
    std::string result;
    for (auto stmt : program->statements) {
        result += printStmt(stmt) + "\n";
    }
    return result;
}
