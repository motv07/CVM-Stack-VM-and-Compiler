CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = main.cpp lexer/lexer.cpp lexer/token.cpp parser/parser.cpp ast/ast_printer.cpp compiler/compiler.cpp vm/vm.cpp vm/instruction.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)