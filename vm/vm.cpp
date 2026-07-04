#include "vm.h"

#include <iostream>

VM::VM()
{
    ip = 0;

    debugMode = true;
    scopeStack.push_back({}); // Initialize global scope
}

void VM::setDebugMode(bool value)
{
    debugMode = value;
}

void VM::push(int value)
{
    stack.push_back(value);
}

int VM::pop()
{
    if (stack.empty())
    {
        std::cout
            << "Runtime Error: Stack underflow"
            << std::endl;

        exit(1);
    }

    int value = stack.back();

    stack.pop_back();

    return value;
}

void VM::printStack()
{
    std::cout << "STACK: ";

    for (int value : stack)
    {
        std::cout
            << value
            << " ";
    }

    std::cout << std::endl;
}

void VM::loadProgram(
    const std::vector<Instruction> &instructions)
{
    program = instructions;
}

void VM::run()
{
    while (ip < program.size())
    {
        Instruction inst =
            program[ip];

        switch (inst.opcode)
        {

        case OP_PUSH:
        {
            push(inst.operand);

            if (debugMode)
            {
                std::cout
                    << "PUSH "
                    << inst.operand
                    << std::endl;
            }

            break;
        }

        case OP_HALT:
        {
            return;
        }

        case OP_ADD:
        {
            int b = pop();

            int a = pop();

            int result = a + b;

            push(result);

            if (debugMode)
            {
                std::cout
                    << "ADD "
                    << a
                    << " + "
                    << b
                    << std::endl;
            }

            break;
        }

        case OP_SUB:
        {
            int b = pop();

            int a = pop();

            int result = a - b;

            push(result);

            if (debugMode)
            {
                std::cout
                    << "SUB "
                    << a
                    << " - "
                    << b
                    << std::endl;
            }

            break;
        }

        case OP_MUL:
        {
            int b = pop();

            int a = pop();

            int result = a * b;

            push(result);

            if (debugMode)
            {
                std::cout
                    << "MUL "
                    << a
                    << " * "
                    << b
                    << std::endl;
            }

            break;
        }

        case OP_DIV:
        {
            int b = pop();

            int a = pop();

            if (b == 0)
            {
                std::cout
                    << "Runtime Error: Division by zero"
                    << std::endl;

                exit(1);
            }

            int result = a / b;

            push(result);

            if (debugMode)
            {
                std::cout
                    << "DIV "
                    << a
                    << " / "
                    << b
                    << std::endl;
            }

            break;
        }

        case OP_PRINT:
        {
            int value = pop();

            std::cout
                << "OUTPUT: "
                << value
                << std::endl;

            break;
        }

        case OP_DEFINE:
        {
            int value = pop();
            // DEFINE strictly creates the variable in the current innermost scope
            scopeStack.back()[inst.name] = value;

            if (debugMode)
                std::cout << "DEFINE " << inst.name << " = " << value << std::endl;
            break;
        }

        case OP_STORE:
        {
            int value = pop();
            bool found = false;

            // STORE searches from innermost scope to global scope
            for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
            {
                if (it->find(inst.name) != it->end())
                {
                    (*it)[inst.name] = value;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                std::cout << "Runtime Error: Cannot assign to undeclared variable '" << inst.name << "'" << std::endl;
                exit(1);
            }

            if (debugMode)
                std::cout << "STORE " << inst.name << " = " << value << std::endl;
            break;
        }

        case OP_LOAD:
        {
            int value = 0;
            bool found = false;
            // Search from innermost scope to global scope
            for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
            {
                if (it->find(inst.name) != it->end())
                {
                    value = (*it)[inst.name];
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                std::cout << "Runtime Error: Undefined variable '" << inst.name << "'" << std::endl;
                exit(1);
            }
            push(value);
            if (debugMode)
                std::cout << "LOAD " << inst.name << " = " << value << std::endl;
            break;
        }

        case OP_ENTER_SCOPE:
        {
            scopeStack.push_back({});
            if (debugMode)
                std::cout << "ENTER_SCOPE" << std::endl;
            break;
        }

        case OP_EXIT_SCOPE:
        {
            if (scopeStack.size() > 1)
            {
                scopeStack.pop_back();
            }
            if (debugMode)
                std::cout << "EXIT_SCOPE" << std::endl;
            break;
        }
        case OP_GT:
        {
            int b = pop();

            int a = pop();

            int result = (a > b);

            push(result);

            if (debugMode)
            {
                std::cout
                    << "GT "
                    << a
                    << " > "
                    << b
                    << std::endl;
            }

            break;
        }

        case OP_LT:
        {
            int b = pop();

            int a = pop();

            int result = (a < b);

            push(result);

            if (debugMode)
            {
                std::cout
                    << "LT "
                    << a
                    << " < "
                    << b
                    << std::endl;
            }

            break;
        }

        case OP_EQ:
        {
            int b = pop();

            int a = pop();

            int result = (a == b);

            push(result);

            if (debugMode)
            {
                std::cout
                    << "EQ "
                    << a
                    << " == "
                    << b
                    << std::endl;
            }

            break;
        }

        case OP_JMP:
        {
            ip = inst.operand;

            if (debugMode)
            {
                std::cout
                    << "JMP to "
                    << ip
                    << std::endl;
            }

            continue;
        }

        case OP_JMP_IF_FALSE:
        {
            int condition = pop();

            if (condition == 0)
            {
                ip = inst.operand;

                if (debugMode)
                {
                    std::cout
                        << "JMP_IF_FALSE to "
                        << ip
                        << std::endl;
                }

                continue;
            }

            if (debugMode)
            {
                std::cout
                    << "Condition True"
                    << std::endl;
            }

            break;
        }
        case OP_AND:
        {
            int b = pop();
            int a = pop();

            push(a && b);

            break;
        }

        case OP_OR:
        {
            int b = pop();
            int a = pop();

            push(a || b);

            break;
        }
        case OP_INPUT:
        {
            int value;

            std::cout << "Enter value: ";

            std::cin >> value;

            push(value);

            break;
        }
        default:
        {
            std::cout
                << "Unknown Instruction"
                << std::endl;
        }
        }

        if (debugMode)
        {
            printStack();

            printVariables();

            std::cout
                << "------------------"
                << std::endl;
        }

        ip++;
    }
}

void VM::printVariables()
{
    std::cout << "VARIABLES: ";

    // Loop through all active scopes (from global down to innermost)
    for (size_t i = 0; i < scopeStack.size(); i++)
    {
        for (auto pair : scopeStack[i])
        {
            std::cout
                << pair.first
                << "="
                << pair.second
                << " ";
        }
    }

    std::cout << std::endl;
}

void VM::resetProgram()
{
    program.clear();
    ip = 0;
    stack.clear();
    // Preserve variable scopes between REPL executions.
}
