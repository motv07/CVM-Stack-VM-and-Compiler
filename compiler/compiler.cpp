#include "compiler.h"
#include <iostream>

void Compiler::emit(
    OpCode opcode,
    int operand,
    std::string name)
{
    instructions.push_back(
        {opcode,
         operand,
         name});
}

std::vector<Instruction>
Compiler::getInstructions()
{
    return instructions;
}

void Compiler::compile(Expr *expr)
{
    if (expr == nullptr)
    {
        std::cout << "Compile Error: Invalid or missing expression." << std::endl;
        exit(1);
    }
    LiteralExpr *literal =
        dynamic_cast<LiteralExpr *>(expr);

    VariableExpr *variable =
        dynamic_cast<VariableExpr *>(expr);

    AssignExpr *assign =
        dynamic_cast<AssignExpr *>(expr);
    InputExpr *inputExpr =
        dynamic_cast<InputExpr *>(expr);

    if (inputExpr)
    {
        emit(OP_INPUT);

        return;
    }
    BinaryExpr *binary =
        dynamic_cast<BinaryExpr *>(expr);

    if (literal)
    {
        emit(
            OP_PUSH,
            literal->value);

        return;
    }

    if (variable)
    {
        emit(
            OP_LOAD,
            0,
            variable->name);

        return;
    }

    // Inside void Compiler::compile(Expr *expr)
    if (assign)
    {
        compile(assign->value);

        // <-- Check the flag we added!
        if (assign->isLet)
        {
            emit(OP_DEFINE, 0, assign->name);
        }
        else
        {
            emit(OP_STORE, 0, assign->name);
        }

        return;
    }
    if (binary)
    {
        compile(binary->left);

        compile(binary->right);

        switch (binary->op)
        {
        case '+':
            emit(OP_ADD);
            break;

        case '-':
            emit(OP_SUB);
            break;

        case '*':
            emit(OP_MUL);
            break;

        case '/':
            emit(OP_DIV);
            break;

        case '>':
            emit(OP_GT);
            break;

        case '<':
            emit(OP_LT);
            break;

        case '=':
            emit(OP_EQ);
            break;

        case '&':
            emit(OP_AND);
            break;

        case '|':
            emit(OP_OR);
            break;
        }

        return;
    }
}

void Compiler::printInstructions()
{
    std::cout
        << "BYTECODE:"
        << std::endl;

    for (
        int i = 0;
        i < instructions.size();
        i++)
    {
        Instruction inst =
            instructions[i];

        std::cout
            << i
            << ": "
            << opcodeToString(
                   inst.opcode);

        if (inst.opcode == OP_PUSH)
        {
            std::cout
                << " "
                << inst.operand;
        }

        if (
            inst.opcode == OP_STORE ||

            inst.opcode == OP_LOAD)
        {
            std::cout
                << " "
                << inst.name;
        }

        if (
            inst.opcode == OP_JMP ||

            inst.opcode ==
                OP_JMP_IF_FALSE)
        {
            std::cout
                << " "
                << inst.operand;
        }

        std::cout
            << std::endl;
    }

    std::cout
        << std::endl;
}

void Compiler::compileStatement(Stmt *stmt)
{
    PrintStmt *printStmt =
        dynamic_cast<PrintStmt *>(stmt);

    if (printStmt)
    {
        compile(
            printStmt->expression);

        emit(OP_PRINT);

        return;
    }

    ExpressionStmt *exprStmt =
        dynamic_cast<ExpressionStmt *>(stmt);

    if (exprStmt)
    {
        compile(
            exprStmt->expression);

        return;
    }

    WhileStmt *whileStmt = dynamic_cast<WhileStmt *>(stmt);
    if (whileStmt)
    {
        int loopStart = instructions.size();
        compile(whileStmt->condition);
        int jumpIfFalsePos = instructions.size();
        emit(OP_JMP_IF_FALSE, 0);

        emit(OP_ENTER_SCOPE); // CREATE SCOPE
        for (Stmt *s : whileStmt->body)
        {
            compileStatement(s);
        }
        emit(OP_EXIT_SCOPE); // DESTROY SCOPE

        emit(OP_JMP, loopStart);
        instructions[jumpIfFalsePos].operand = instructions.size();
        return;
    }

    IfStmt *ifStmt = dynamic_cast<IfStmt *>(stmt);
    if (ifStmt)
    {
        compile(ifStmt->condition);
        int jumpIfFalsePos = instructions.size();
        emit(OP_JMP_IF_FALSE, 0);

        emit(OP_ENTER_SCOPE); // CREATE SCOPE
        for (Stmt *s : ifStmt->thenBranch)
        {
            compileStatement(s);
        }
        emit(OP_EXIT_SCOPE); // DESTROY SCOPE

        int jumpPos = instructions.size();
        emit(OP_JMP, 0);
        instructions[jumpIfFalsePos].operand = instructions.size();

        if (!ifStmt->elseBranch.empty())
        {
            emit(OP_ENTER_SCOPE); // CREATE SCOPE
            for (Stmt *s : ifStmt->elseBranch)
            {
                compileStatement(s);
            }
            emit(OP_EXIT_SCOPE); // DESTROY SCOPE
        }

        instructions[jumpPos].operand = instructions.size();
        return;
    }
    BlockStmt *blockStmt = dynamic_cast<BlockStmt *>(stmt);
    if (blockStmt)
    {
        emit(OP_ENTER_SCOPE);
        for (Stmt *s : blockStmt->body)
        {
            compileStatement(s);
        }
        emit(OP_EXIT_SCOPE);
        return;
    }
}

void Compiler::compileProgram(
    Program *program)
{
    for (
        Stmt *stmt :
        program->statements)
    {
        compileStatement(stmt);
    }

    emit(OP_HALT);
}

void Compiler::compileAndRun(
    Expr *expr)
{
    compile(expr);

    AssignExpr *assign =
        dynamic_cast<AssignExpr *>(expr);

    if (assign)
    {
        emit(
            OP_LOAD,
            0,
            assign->name);
    }

    emit(OP_PRINT);

    emit(OP_HALT);
}