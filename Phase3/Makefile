#     --- MAKEFILE ---
#    Georgios Zervos AM:3384
#	 Stylianos Michelakakis AM:3524
#	 Iasonas Filippos Ntagiannis AM:3540 
#    Compiled and run in Mac OS Big Sur 11.3.1 , x86 chip
CC= g++
FL=flex
BS=bison
BSFLAGS= -v --yacc --defines="parser.h"
OFLAGS=-o
RUN=./
REM=rm -f

all:	lexical_analyzer.cpp syntax_analyzer.cpp SymbolTable_v2.cpp iCode.cpp
	$(CC) $(OFLAGS) alpha_icode lexical_analyzer.cpp syntax_analyzer.cpp SymbolTable_v2.cpp iCode.cpp

lexical_analyzer.cpp:	lex_an_v2.l
	$(FL)  lex_an_v2.l
	
syntax_analyzer.cpp:	syntax_an_v2.y
	$(BS) $(BSFLAGS)  syntax_an_v2.y

clean:  syntax_analyzer.cpp lexical_analyzer.cpp parser.h alpha_icode.output alpha_icode
	$(REM)   syntax_analyzer.cpp lexical_analyzer.cpp parser.h alpha_icode.output alpha_icode

