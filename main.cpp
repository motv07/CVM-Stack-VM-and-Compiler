#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "compiler/compiler.h"
#include "vm/vm.h"
#include "ast/ast_printer.h"

using namespace std;

int main(int argc, char *argv[])
{
    bool debugMode = false;
    std::string filename = "";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--debug") {
            debugMode = true;
        } else {
            filename = arg;
        }
    }

    if (!filename.empty())
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Could not open file" << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string source = buffer.str();

        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();

        Parser parser(tokens);
        Program *program = parser.parse();

        if (debugMode) {
            std::cout << "=== AST ===" << std::endl;
            ASTPrinter printer;
            std::cout << printer.print(program);
            std::cout << "=== BYTECODE ===" << std::endl;
        }

        Compiler compiler;
        compiler.compileProgram(program);
        std::vector<Instruction> code = compiler.getInstructions();

        VM vm;
        vm.setDebugMode(debugMode);
        vm.loadProgram(code);
        vm.run();
        return 0;
    }

    cout << "CVM++ REPL" << endl;
    cout << "Type 'exit' to quit" << endl;

    VM vm;
    vm.setDebugMode(false);

    while (true)
    {
        cout << ">> ";
        string source;
        getline(cin, source);
        if (source == "exit") break;

        Lexer lexer(source);
        vector<Token> tokens = lexer.scanTokens();
        Parser parser(tokens);
        Program *program = parser.parse();
        Compiler compiler;
        compiler.compileProgram(program);
        vector<Instruction> code = compiler.getInstructions();
        vm.resetProgram();
        vm.loadProgram(code);
        vm.run();
    }
    return 0;
}
