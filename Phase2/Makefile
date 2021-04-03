
CC= g++

FL=flex
BS=bison
BSFLAGS= -v --yacc --defines="parser.h"
OFLAGS=-o


REM=rm -f


all:	lexical_analyzer.cpp syntax_analyzer.cpp SymbolTable.cpp
	$(CC) $(OFLAGS) syntax_analyzer lexical_analyzer.cpp syntax_analyzer.cpp SymbolTable.cpp

lexical_analyzer.cpp:	lex_an_v2.l
	$(FL)  lex_an_v2.l
	
syntax_analyzer.cpp:	syntax_an.y
	$(BS) $(BSFLAGS)  syntax_an.y

clean:  syntax_analyzer.cpp lexical_analyzer.cpp parser.h syntax_analyzer.output
	$(REM)   syntax_analyzer.cpp lexical_analyzer.cpp parser.h syntax_analyzer.output

