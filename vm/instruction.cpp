#include "instruction.h"

std::string opcodeToString(OpCode opcode)
{
    switch (opcode)
    {
    case OP_PUSH:
        return "PUSH";

    case OP_ADD:
        return "ADD";

    case OP_SUB:
        return "SUB";

    case OP_MUL:
        return "MUL";

    case OP_DIV:
        return "DIV";

    case OP_PRINT:
        return "PRINT";

    case OP_LOAD:
        return "LOAD";

    case OP_GT:
        return "GT";

    case OP_LT:
        return "LT";

    case OP_EQ:
        return "EQ";

    case OP_JMP:
        return "JMP";

    case OP_JMP_IF_FALSE:
        return "JMP_IF_FALSE";

    case OP_HALT:
        return "HALT";
    case OP_AND:
        return "AND";

    case OP_OR:
        return "OR";
    case OP_STORE:
        return "STORE";
    case OP_DEFINE:
        return "DEFINE"; // <-- ADD THIS
    case OP_ENTER_SCOPE:
        return "ENTER_SCOPE";
    case OP_EXIT_SCOPE:
        return "EXIT_SCOPE";
    default:
        return "UNKNOWN";
    }
}