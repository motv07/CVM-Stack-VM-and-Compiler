#pragma once

#include <iostream>
#include <vector>

#include "../vm/instruction.h"
#include "../ast/expr.h"
#include "../ast/stmt.h"

class Compiler
{

private:
    std::vector<Instruction> instructions;

public:
    void compile(Expr *expr);
    void compileStatement(Stmt *stmt);

    void compileProgram(Program *program);
    void compileAndRun(Expr *expr);

    void emit(OpCode opcode,
              int operand = 0,
              std::string name = "");

    std::vector<Instruction> getInstructions();

    void printInstructions();
};