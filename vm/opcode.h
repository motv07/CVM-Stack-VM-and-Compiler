#pragma once

enum OpCode
{

    // Stack Operations
    OP_PUSH,
    OP_POP,

    // Arithmetic Operations
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,

    // Variables
    OP_STORE,
    OP_LOAD,
    OP_DEFINE,
    // Output
    OP_PRINT,

    // Comparisons
    OP_GT,
    OP_LT,
    OP_EQ,
    OP_AND,
    OP_OR,
    // Control Flow
    OP_JMP,
    OP_JMP_IF_FALSE,
    OP_INPUT,
    // Program Control
    OP_HALT,

    OP_ENTER_SCOPE,
    OP_EXIT_SCOPE
};