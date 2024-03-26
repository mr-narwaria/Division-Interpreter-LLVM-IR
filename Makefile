.PHONY: clean

all: division-interpreter 

division-interpreter: Parser.o Tokenizer.o ASTNode.o Main.o
	@g++ -o division-interpreter -std=c++14 Main.o Parser.o ASTNode.o Tokenizer.o
	@echo "division-interpreter compiled successfully"

Main.o: Main.cpp
	@g++ -std=c++14 -c Main.cpp

Parser.o: Parser.cpp
	@g++ -std=c++14 -c Parser.cpp

Tokenizer.o: Tokenizer.cpp
	@g++ -std=c++14 -c Tokenizer.cpp


ASTNode.o: ASTNode.cpp
	@g++ -std=c++14 -c ASTNode.cpp

clean:
	@rm -f *.o division-interpreter *.txt *.ll
