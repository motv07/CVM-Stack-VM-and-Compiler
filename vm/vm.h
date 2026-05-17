#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "instruction.h"

class VM
{

private:
    // Stack for execution
    std::vector<int> stack;

    // Program instructions
    std::vector<Instruction> program;

    // Variables storage (Now a stack of scopes)
    std::vector<std::unordered_map<std::string, int>> scopeStack;

    // Instruction Pointer
    int ip;
    int pop();

    void push(int value);

    void printStack();
    void printVariables();
    bool debugMode;

public:
    VM();

    void loadProgram(const std::vector<Instruction> &instructions);
    void resetProgram();
    void setDebugMode(bool value);
    void run();
};