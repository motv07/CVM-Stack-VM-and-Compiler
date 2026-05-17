#pragma once

#include "opcode.h"
#include <string>

struct Instruction
{

    OpCode opcode;

    int operand;

    std::string name;
};
std::string opcodeToString(OpCode opcode);